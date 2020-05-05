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
      DashboardStore.CountOriginationAndReveal(
        countContracts,
        countOriginations,
      )
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
        DashboardStore.GetStorageDelta24h(value) |> toOption |> Future.value
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
        DashboardStore.GetTop3Bakers(value) |> toOption |> Future.value
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
    // getLastDayTransactions(~startDate, ~endDate=timestamp, ~config),
    // getLastDayZeroPriorityBlocks(~startDate, ~endDate=timestamp, ~config),
    // getLastDayBakersWithOutput(~startDate, ~endDate=timestamp, ~config),
    getLastDayOriginationAndReveal(~startDate, ~endDate=timestamp, ~config),
    getTop3Bakers(~config),
    getLastDayStorageDelta(~startDate, ~endDate=timestamp, ~config),
  ])
  ->Future.map(
      List.map(
        fun
        | Some(DashboardStore.CountedTransactions24h(value)) =>
          Js.log2("Counted transactions", value)
        | Some(DashboardStore.CountedZeroPriorityBlocksLevels24h(value)) =>
          Js.log2("Counted zero priority blocks levels", value)
        | Some(DashboardStore.CountedBakers24h(value)) =>
          Js.log2("Counted bakers with output", value)
        | Some(
            DashboardStore.CountOriginationAndReveal(
              countContracts,
              countOriginations,
            ),
          ) =>
          Js.log3(
            "Counted contracts and originations",
            countContracts,
            countOriginations,
          )
        | Some(DashboardStore.GetTop3Bakers(value)) =>
          Js.log2("top 3 bakers", value)
        | Some(DashboardStore.GetStorageDelta24h(value)) =>
          Js.log2("storage delta", value)
        | _ => Js.log("unknown value"),
      ),
    )
  ->Future.get(callback);
};