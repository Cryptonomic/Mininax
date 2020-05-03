open DashboardStore;
open Decode;
open Helpers;

let reduceBlockInfo =
  List.fold_left(
    (accumulative, current) =>
      switch (current) {
      | BlockInfoFailed => accumulative
      | BlockCount(value) => {blockCount: Some(value)}
      },
    initBlockInfo,
  );

let getNumBlocks =
    (~metaCycle: int, ~timestamp: float, ~config: MainType.config) =>
  ApiCall.getForQueryApi(
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

let getBlockInfoThunk =
    (~callback, ~metaCycle: int, ~timestamp: float, ~config: MainType.config) =>
  Future.all([
    // ApiCall.getForQueryApi(
    //   ~query=Queries.getQueryForNumBlocks(metaCycle, timestamp),
    //   ~field="blocks",
    //   ~config,
    // ),
    getNumBlocks(~metaCycle, ~timestamp, ~config),
  ])
  ->Future.map(reduceBlockInfo)
  ->Future.get(callback);