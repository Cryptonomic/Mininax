open DashboardStore.ApiReducers;
open DashboardStore.Types_;
open DashboardStore.Inits;

open Decode;
open Helpers;

module Fold = {
  let reduceBlockInfo =
    List.fold_left(
      (accumulative, current) =>
        switch (current) {
        | BlockInfoFailed => accumulative
        | BlockCount(value) => {...accumulative, blockCount: Some(value)}
        | CountedZeroPriorityBlocksLevels(zeroPriorityBlocks) => {
            ...accumulative,
            zeroPriorityBlocks,
          }
        | CountedBakers(bakersWithOutput) => {
            ...accumulative,
            bakersWithOutput,
          }
        },
      initBlockInfo,
    );

  let reduceTotalsInfo =
    List.fold_left(
      (accumulative, current) => {
        switch (current) {
        | TotalInfoFailed => accumulative
        | AmountAndContracts(value) => {
            ...accumulative,
            countAmount: value.countAmount,
            sumAmount: value.sumAmount,
            countOriginatedContracts: value.countOriginatedContracts,
          }
        | FundraiserCount(fundraiserCount) => {
            ...accumulative,
            fundraiserCount,
          }
        | TotalFundraiserCount(totalFundraiserCount) => {
            ...accumulative,
            totalFundraiserCount,
          }
        | SumFeeGasDelta(value) => {
            ...accumulative,
            sumFee: value.sumFee |> toOption,
            sumConsumedGas: value.sumConsumedGas |> toOption,
            storageDelta: value.storageDelta |> toOption,
          }
        | CountedTransactions(countTransactions) => {
            ...accumulative,
            countTransactions,
          }
        | ActivationOriginationReveal(value) => {
            ...accumulative,
            fundraiserCount: value.activatedFundraiserCount,
            reveals: value.reveal,
            contractDeployed: value.contractDeployed,
          }
        }
      },
      initTotalsInfo,
    );

  let reduceGevernanceInfo =
    List.fold_left(
      (accumulative, current) => {
        switch (current) {
        | GovernanceProcessInfoFailed => accumulative
        | CurrentExpectedQuorum(currentExpectedQuorum) => {
            ...accumulative,
            currentExpectedQuorum,
          }
        | VoteInfo(info) => {
            ...accumulative,
            yayRolls: info.yayRolls,
            nayRolls: info.nayRolls,
            passRolls: info.passRolls,
            proposalHash: info.proposalHash,
          }
        }
      },
      initGovernanceProcessInfo,
    );

  let reduceBakersInfo =
    List.fold_left(
      (accumulative, current) => {
        switch (current) {
        | BakersInfoFailed => accumulative
        | BakersSumStakingBalance(bakersSumStakingBalance) => {
            ...accumulative,
            bakersSumStakingBalance,
          }
        | Top3Bakers(top3Bakers) => {...accumulative, top3Bakers}
        | TotalTez(totalTez) => {...accumulative, totalTez}
        }
      },
      intBakersInfo,
    );
};

module Calls = {
  let getNumBlocks =
      (~metaCycle: int, ~timestamp: float, ~config: MainType.config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForNumBlocks(metaCycle, timestamp),
      ~field="blocks",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) => value |> json_of_magic |> parseBlockCount |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => BlockCount(value) |> Future.value
        | _ => BlockInfoFailed |> Future.value,
      );

  let getAmountAndContracts = (~timestamp, ~config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForTransactionStats(timestamp),
      ~field="operations",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) =>
          value |> json_of_magic |> parseAmountAndContracts |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => AmountAndContracts(value) |> Future.value
        | _ => TotalInfoFailed |> Future.value,
      );

  let getFundraiserStats = (~timestamp, ~config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForFundraiserStats(timestamp),
      ~field="operations",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) =>
          value |> json_of_magic |> parseFundraiserStats |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => FundraiserCount(value) |> Future.value
        | _ => TotalInfoFailed |> Future.value,
      );

  let getQuorumInfo = (~hash: string, ~config: MainType.config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForQuorum(hash),
      ~field="blocks",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) => value |> json_of_magic |> parseQuorumInfo |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(quorumInfo) =>
          CurrentExpectedQuorum(Some(quorumInfo)) |> Future.value
        | _ => GovernanceProcessInfoFailed |> Future.value,
      );

  let getVoteInfo =
      (~hash: string, ~active_proposal: string, ~config: MainType.config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForVotingStats(hash, active_proposal),
      ~field="governance",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) => value |> json_of_magic |> parseVoteInfo |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(vInfo) => VoteInfo(vInfo) |> Future.value
        | _ => GovernanceProcessInfoFailed |> Future.value,
      );

  let getSumStakingBalance = (~config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForBakerStats(),
      ~field="bakers",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) =>
          value |> json_of_magic |> parseSumStakingBalance |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => BakersSumStakingBalance(value) |> Future.value
        | _ => BakersInfoFailed |> Future.value,
      );

  let getSumTez = (~config) =>
    GlobalApi.getForQueryApi(
      ~query=Queries.getQueryForMarketCap(),
      ~field="accounts",
      ~config,
    )
    ->Future.map(
        fun
        | Some(value) => value |> json_of_magic |> parseSumTez |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => TotalTez(value) |> Future.value
        | _ => BakersInfoFailed |> Future.value,
      );

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
        | Some(value) => CountedTransactions(Some(value)) |> Future.value
        | _ => TotalInfoFailed |> Future.value,
      );

  let getLastDayZeroPriorityBlocks =
      (~startDate: float, ~endDate: float, ~config: MainType.config) =>
    ConseiljsRe.ConseilDataClient.executeEntityQuery
    ->applyTuple3(~tuple=Utils.getInfo(config))
    ->applyField(~field="blocks")
    ->applyQuery(
        ~query=
          Queries.getQueryForZeroPriorityBlocksLast24(startDate, endDate),
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
          CountedZeroPriorityBlocksLevels(Some(value)) |> Future.value
        | _ => BlockInfoFailed |> Future.value,
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
        | Some(value) => CountedBakers(Some(value)) |> Future.value
        | _ => BlockInfoFailed |> Future.value,
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
          |> json_of_magic
          |> Json.Decode.array(countOriginationsAndReveals)
          |> toOption
        | _ => None,
      )
    ->Future.map(
        fun
        | Some(value) => {
            value
            |> Array.fold_left(
                 (accu, curr: originationAndReveals) => {
                   switch (curr.kind, curr.status) {
                   | (Reveal, Applied) => {
                       ...accu,
                       reveal: curr.countOperation |> toOption,
                     }
                   | (Origination, Applied) => {
                       ...accu,
                       contractDeployed: curr.countOperation |> toOption,
                     }
                   | (ActivateAccount, _) => {
                       ...accu,
                       activatedFundraiserCount:
                         curr.countOperation |> toOption,
                     }
                   | _ => accu
                   }
                 },
                 intActivationOriginationReveal,
               )
            |> toOption;
          }
        | _ => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => ActivationOriginationReveal(value) |> Future.value
        | None => TotalInfoFailed |> Future.value,
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
          value |> json_of_magic |> Json.Decode.array(parseBaker) |> toOption
        | _ => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => Top3Bakers(Some(value)) |> Future.value
        | _ => BakersInfoFailed |> Future.value,
      );

  let getLastDayStorageDeltaSumFeeAndGas =
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
          value[0] |> json_of_magic |> getStorageDelta |> toOption
        | _ => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => SumFeeGasDelta(value) |> Future.value
        | _ => TotalInfoFailed |> Future.value,
      );

  let getTheLatestGovernance = (~config: MainType.config) =>
    ConseiljsRe.ConseilDataClient.executeEntityQuery
    ->applyTuple3(~tuple=Utils.getInfo(config))
    ->applyField(~field="governance")
    ->applyQuery(~query=Queries.getQueryForTheLatestGovernance())
    ->FutureJs.fromPromise(_err => None)
    ->Future.map(
        fun
        | Ok(value) when value |> Array.length > 0 =>
          value[0] |> json_of_magic |> parseLatestGovernance |> toOption
        | _ => None,
      )
    ->Future.flatMap(
        fun
        | Some(value) => Some(value) |> Future.value
        | _ => None |> Future.value,
      );
};

module Thunk = {
  open Calls;
  open Fold;
  let getProposalsInfoThunk = (~metaCycle: int, ~config: MainType.config) =>
    ConseiljsRe.ConseilDataClient.executeEntityQuery
    ->applyTuple3(~tuple=Utils.getInfo(config))
    ->applyField(~field="operations")
    ->applyQuery(~query=Queries.getQueryForProposalInfo(metaCycle))
    ->FutureJs.fromPromise(_err => None)
    ->Future.map(
        fun
        | Ok(proposalStats) when proposalStats |> Array.length > 0 =>
          proposalStats |> json_of_magic |> parseProposalsInfo |> toOption
        | Ok(_) => Some([||])
        | _err => None,
      );

  let getGovernanceProcessInfoThunk =
      (~hash: string, ~active_proposal: string, ~config: MainType.config) =>
    Future.all([
      getQuorumInfo(~hash, ~config),
      getVoteInfo(~hash, ~active_proposal, ~config),
    ])
    ->Future.map(reduceGevernanceInfo);

  let getBakersInfoThunk = (~config: MainType.config) =>
    Future.all([
      getSumStakingBalance(~config),
      getTop3Bakers(~config),
      getSumTez(~config),
    ])
    ->Future.map(reduceBakersInfo);

  let getTotalsInfoThunk = (~timestamp: float, ~config: MainType.config) => {
    let startDate =
      timestamp
      |> MomentRe.momentWithTimestampMS
      |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
      |> MomentRe.Moment.valueOf;
    Future.all([
      getAmountAndContracts(~timestamp, ~config),
      getFundraiserStats(~timestamp, ~config),
      getLastDayStorageDeltaSumFeeAndGas(
        ~startDate,
        ~endDate=timestamp,
        ~config,
      ),
      getLastDayTransactions(~startDate, ~endDate=timestamp, ~config),
      getLastDayOriginationAndReveal(~startDate, ~endDate=timestamp, ~config),
    ])
    ->Future.map(reduceTotalsInfo);
  };

  let getBlockInfoThunk =
      (~metaCycle: int, ~timestamp: float, ~config: MainType.config) => {
    let startDate =
      timestamp
      |> MomentRe.momentWithTimestampMS
      |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
      |> MomentRe.Moment.valueOf;
    Future.all([
      getNumBlocks(~metaCycle, ~timestamp, ~config),
      getLastDayZeroPriorityBlocks(~startDate, ~endDate=timestamp, ~config),
      getLastDayBakersWithOutput(~startDate, ~endDate=timestamp, ~config),
    ])
    ->Future.map(reduceBlockInfo);
  };
};