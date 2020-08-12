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
    ->applyField(~field="bakers")
    ->applyQuery(~query=Queries.getQueryForBakerInfo(id))
    ->FutureJs.fromPromise(_err => None)
    ->Future.flatMap(
        fun
        | Ok(bakers) when bakers |> Js.Array.length > 0 =>
          Future.value(Some(bakers[0]))
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

let getBlockHeadThunk = (~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getBlockHead
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(result) => Some(result)
      | _err => None,
    );

let getBlockThunk = (~id: string, ~config: MainType.config) =>
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
    );

let getBlockHashThunk = (~level: int, ~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getBlockByLevel
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->apply1(level)
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(
      fun
      | Ok(hashes) when hashes |> Js.Array.length > 0 => Some(hashes[0])
      | _err => None,
    );

let getOperationThunk = (~id: string, ~config: MainType.config) =>
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
    );

let getAccountThunk = (~id: string, ~config: MainType.config) =>
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
    );

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
