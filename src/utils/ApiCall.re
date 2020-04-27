open Helpers;

module ReturningPromise = {
  let labelExecuteEntityQuery = (info, platform, network, ~field, query) =>
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      info,
      platform,
      network,
      field,
      query,
    );

  let labelGetBlock = (conseilServerInfo, network, ~id) =>
    ConseiljsRe.TezosConseilClient.getBlock(conseilServerInfo, network, id);

  let getBlockTotalsThunk = (~id: string, ~config: MainType.config) =>
    labelExecuteEntityQuery(~field="operations")
    ->applyTuple3(~tuple=Utils.getInfo(config))
    ->applyQuery(~query=Queries.getQueryForBlockTotals(id))
    ->FutureJs.fromPromise(_err => None)
    ->Future.flatMap(
        fun
        | Ok(totals) when totals |> Js.Array.length > 0 =>
          Future.value(Some(totals[0]))
        | _err => Future.value(None),
      );

  let getBlockFromApi = (~id: string, ~config: MainType.config) =>
    labelGetBlock(~id)
    ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
    ->FutureJs.fromPromise(_err => None)
    ->Future.flatMap(
        fun
        | Ok(block) => Future.value(Some(block))
        | _err => Future.value(None),
      );

  let getAccountBakerThunk = (~id: string, ~config: MainType.config) =>
    ConseiljsRe.ConseilDataClient.executeEntityQuery
    ->applyTuple3(~tuple=Utils.getInfo(config))
    ->applyField(~field="delegates")
    ->applyQuery(~query=Queries.getQueryForBakerInfo(id))
    ->FutureJs.fromPromise(_err => None)
    ->Future.flatMap(
        fun
        | Ok(delegates) when delegates |> Js.Array.length > 0 =>
          Future.value(Some(delegates[0]))
        | _err => Future.value(None),
      );

  let getAccountFromApi = (~id: string, ~config: MainType.config) =>
    ConseiljsRe.TezosConseilClient.getAccount
    ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
    ->apply1(id)
    ->FutureJs.fromPromise(_err => None)
    ->Future.flatMap(
        fun
        | Ok(account) => Some(account) |> Future.value
        | _err => None |> Future.value,
      );
};

open ReturningPromise;

let getBlockHeadThunk = (~callback, ~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getBlockHead
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(result) => Some(result)
      | _err => None,
    )
  ->Future.get(callback);

let getBlockThunk = (~callback, ~id: string, ~config: MainType.config) =>
  Js.Promise.all2((
    FutureJs.toPromise(getBlockFromApi(~id, ~config)),
    FutureJs.toPromise(getBlockTotalsThunk(~id, ~config)),
  ))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok((Some(block), Some(total))) => {
          let realBlock = Convert.convertBlock(~block, ~total, ());
          Ok((realBlock, block));
        }
      | Ok((Some(block), None)) => {
          let realBlock = Convert.convertBlock(~block, ());
          Ok((realBlock, block));
        }
      | Ok((None, None)) => Error(ErrMessage.noAvailable)
      | _err => Error(ErrMessage.invalidId),
    )
  ->Future.get(callback);

let getBlockHashThunk = (~callback, ~level: int, ~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getBlockByLevel
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->apply1(level)
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(hashes) when hashes |> Js.Array.length > 0 => Some(hashes[0])
      | _err => None,
    )
  ->Future.get(callback);

let getOperationThunk = (~callback, ~id: string, ~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getOperations
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->applyQuery(~query=Queries.getQueryForOperations(id))
  ->FutureJs.fromPromise(_err => ErrMessage.noAvailable)
  ->Future.map(
      fun
      | Ok(operations) when operations |> Js.Array.length > 0 =>
        operations |> Array.map(Convert.convertOperation) |> toResult
      | Ok(_) => Error(ErrMessage.noAvailable)
      | Error(e) => Error(e),
    )
  ->Future.get(callback);

let getAccountThunk = (~callback, ~id: string, ~config: MainType.config) =>
  Js.Promise.all2((
    getAccountFromApi(~id, ~config) |> FutureJs.toPromise,
    getAccountBakerThunk(~id, ~config) |> FutureJs.toPromise,
  ))
  ->FutureJs.fromPromise(_err => ErrMessage.noAvailable)
  ->Future.map(
      fun
      | Ok((Some(account), Some(baker))) =>
        Convert.convertAccount(~account, ~baker, ()) |> toResult
      | Ok((Some(account), None)) =>
        Convert.convertAccount(~account, ()) |> toResult
      | Ok(_) when id |> slice(~to_=2) == "tz" =>
        Error(ErrMessage.missingAccount)
      | Ok(_) when id |> slice(~to_=2) == "kt" =>
        Error(ErrMessage.missingContract)
      | Ok(_) => Error(ErrMessage.invalidAccountId)
      | Error(err) => Error(err),
    )
  ->Future.get(callback);

let getForQueryApi = (~query, ~field: string, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field)
  ->applyQuery(~query)
  ->FutureJs.fromPromise(_err => None)
  ->Future.flatMap(
      fun
      | Ok(value) when value |> Js.Array.length > 0 =>
        value[0] |> toOption |> Future.value
      | _ => None |> Future.value,
    );

let getVoteInfoThunk =
    (
      ~callback,
      ~hash: string,
      ~active_proposal: string,
      ~config: MainType.config,
    ) =>
  Js.Promise.all2((
    getForQueryApi(
      ~query=Queries.getQueryForQuorum(hash),
      ~field="blocks",
      ~config,
    )
    |> FutureJs.toPromise,
    getForQueryApi(
      ~query=Queries.getQueryForVotingStats(hash, active_proposal),
      ~field="governance",
      ~config,
    )
    |> FutureJs.toPromise,
  ))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok((Some(quorumStat), Some(votingStat))) => {
          let quorumObj = quorumStat |> Obj.magic;
          let votingObj = votingStat |> Obj.magic;
          let votinfo: MainType.voteInfo = {
            yay_rolls: votingObj##yay_rolls,
            nay_rolls: votingObj##nay_rolls,
            pass_rolls: votingObj##pass_rolls,
            proposal_hash: votingObj##proposal_hash,
            current_expected_quorum: quorumObj##current_expected_quorum,
          };
          Some(votinfo);
        }
      | _ => None,
    )
  ->Future.get(callback);

let getProposalInfoThunk =
    (~callback, ~metaCycle: int, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="operations")
  ->applyQuery(~query=Queries.getQueryForProposalInfo(metaCycle))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(proposalStats) when proposalStats |> Array.length > 0 =>
        proposalStats
        |> Array.map(proposal => {
             let proposalObj = proposal |> Obj.magic;
             let newProposal: MainType.proposalInfo = {
               count_operation_group_hash:
                 proposalObj##count_operation_group_hash,
               proposal: proposalObj##proposal,
             };
             newProposal;
           })
        |> toOption
      | Ok(_) => Some([||])
      | _err => None,
    )
  ->Future.get(callback);

let getBlockInfoThunk =
    (~callback, ~metaCycle: int, ~timestamp: float, ~config: MainType.config) =>
  Future.all([
    getForQueryApi(
      ~query=Queries.getQueryForNumBlocks(metaCycle, timestamp),
      ~field="blocks",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForTransactionStats(timestamp),
      ~field="operations",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForFundraiserStats(timestamp),
      ~field="operations",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForTotalFundraiserActivated(),
      ~field="operations",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForFeesStats(timestamp),
      ~field="operations",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForBakerStats(),
      ~field="delegates",
      ~config,
    ),
    getForQueryApi(
      ~query=Queries.getQueryForMarketCap(),
      ~field="accounts",
      ~config,
    ),
  ])
  ->Future.map(
      fun
      | [res1, res2, res3, res4, res5, res6, res7] => {
          let newTransInfoObj = res2 |> Obj.magic;
          let newTranInfo: MainType.transInfo = {
            countOriginatedContracts:
              newTransInfoObj##count_originated_contracts,
            countAmount: newTransInfoObj##count_amount,
            sumAmount: newTransInfoObj##sum_amount,
          };
          let newFundraiser = res3 |> Obj.magic;
          let newTotalFundraiser = res4 |> Obj.magic;
          let feesObj = res5 |> Obj.magic;
          let bakersObj = res6 |> Obj.magic;
          let marketObj = res7 |> Obj.magic;

          let newBlockInfo: MainType.blockInfo = {
            blockCount: (res1 |> Obj.magic)##count_hash,
            fundraiserCount: newFundraiser##count_kind,
            totalFundraiserCount: newTotalFundraiser##count_kind,
            sum_fee: feesObj##sum_fee,
            sum_consumed_gas: feesObj##sum_consumed_gas,
            num_bakers: bakersObj##count_pkh,
            bakers_sum_staking_balance: bakersObj##sum_staking_balance,
            totalTez: marketObj##sum_balance,
          };
          Some((newBlockInfo, newTranInfo));
        }
      | _ => None,
    )
  ->Future.get(callback);

let getLastDayTransactions =
    (~startDate: float, ~endDate: float, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="operations")
  ->applyQuery(
      ~query=Queries.getQueryForLastDayTransactions(startDate, endDate),
    )
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value[0]
        |> Decode.json_of_magic
        |> Decode.countedTransactions
        |> toOption
      | _ => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) =>
        MainType.CountedTransactions24h(value) |> toOption |> Future.value
      | _ => None |> Future.value,
    );

let getLastDayZeroPriorityBlocks =
    (~startDate: float, ~endDate: float, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="blocks")
  ->applyQuery(
      ~query=Queries.getQueryForZeroPriorityBlocksLast24(startDate, endDate),
    )
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value[0]
        |> Decode.json_of_magic
        |> Decode.countedZeroPriorityBlocksLevels
        |> toOption
      | _ => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) =>
        MainType.CountedZeroPriorityBlocksLevels24h(value)
        |> toOption
        |> Future.value
      | _ => None |> Future.value,
    );

let getLastDayBakersWithOutput =
    (~startDate: float, ~endDate: float, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="blocks")
  ->applyQuery(
      ~query=Queries.getQueryForBakersWithOutput(startDate, endDate),
    )
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value[0] |> Decode.json_of_magic |> Decode.countedBaker |> toOption
      | _ => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) =>
        MainType.CountedBakers24h(value) |> toOption |> Future.value
      | _ => None |> Future.value,
    );

let getLastDayOriginationAndReveal =
    (~startDate: float, ~endDate: float, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="operations")
  ->applyQuery(
      ~query=
        Queries.getQueryForOriginationAndRevealLastDay(startDate, endDate),
    )
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value
        |> Decode.json_of_magic
        |> Json.Decode.array(Decode.countOriginationsAndReveals)
        |> toOption
      | _ => None,
    )
  ->Future.map(
      fun
      | Some(value) => {
          value
          |> Array.fold_left(
               (accu, curr: Decode.originationAndReveals) => {
                 let (countContracts, countOrigination) = accu;
                 switch (curr.kind, curr.status) {
                 | (Decode.Reveal, _) => (
                     curr.countOperation |> toOption,
                     countOrigination,
                   )
                 | (Decode.Origination, Decode.Applied) => (
                     countContracts,
                     curr.countOperation |> toOption,
                   )
                 | _ => accu
                 };
               },
               (None, None),
             );
        }
      | _ => (None, None),
    )
  ->Future.flatMap(value => {
      let (countContracts, countOriginations) = value;
      MainType.CountOriginationAndReveal(countContracts, countOriginations)
      |> toOption
      |> Future.value;
    });

let getLastDayStorageDelta =
    (~startDate: float, ~endDate: float, ~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="operations")
  ->applyQuery(
      ~query=Queries.getQueryForStorageDeltaLastDay(startDate, endDate),
    )
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value[0] |> Decode.json_of_magic |> Decode.getStorageDelta |> toOption
      | _ => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) =>
        MainType.GetStorageDelta24h(value) |> toOption |> Future.value
      | _ => None |> Future.value,
    );

let getTop3Bakers = (~config: MainType.config) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyField(~field="accounts")
  ->applyQuery(~query=Queries.getQueryForTop3BakersLastDay())
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(value) when value |> Array.length > 0 =>
        value
        |> Decode.json_of_magic
        |> Json.Decode.array(Decode.getBaker)
        |> toOption
      | _ => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) =>
        MainType.GetTop3Bakers(value) |> toOption |> Future.value
      | _ => None |> Future.value,
    );

let getExtraOtherTotals =
    (~callback, ~timestamp: float, ~config: MainType.config) => {
  let startDate =
    timestamp
    |> MomentRe.momentWithTimestampMS
    |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
    |> MomentRe.Moment.valueOf;
  Future.all([
    getLastDayTransactions(~startDate, ~endDate=timestamp, ~config),
    getLastDayZeroPriorityBlocks(~startDate, ~endDate=timestamp, ~config),
    getLastDayBakersWithOutput(~startDate, ~endDate=timestamp, ~config),
    getLastDayOriginationAndReveal(~startDate, ~endDate=timestamp, ~config),
    getTop3Bakers(~config),
    getLastDayStorageDelta(~startDate, ~endDate=timestamp, ~config),
  ])
  ->Future.map(
      List.map(
        fun
        | Some(MainType.CountedTransactions24h(value)) =>
          Js.log2("Counted transactions", value)
        | Some(MainType.CountedZeroPriorityBlocksLevels24h(value)) =>
          Js.log2("Counted zero priority blocks levels", value)
        | Some(MainType.CountedBakers24h(value)) =>
          Js.log2("Counted bakers with output", value)
        | Some(
            MainType.CountOriginationAndReveal(
              countContracts,
              countOriginations,
            ),
          ) =>
          Js.log3(
            "Counted contracts and originations",
            countContracts,
            countOriginations,
          )
        | Some(MainType.GetTop3Bakers(value)) =>
          Js.log2("top 3 bakers", value)
        | Some(MainType.GetStorageDelta24h(value)) =>
          Js.log2("storage delta", value)
        | _ => Js.log("unknown value"),
      ),
    )
  ->Future.get(callback);
};