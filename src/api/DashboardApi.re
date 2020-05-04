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

let getAmountAndContracts = (~timestamp, ~config) =>
  ApiCall.getForQueryApi(
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
      | Some(value) => {
          Js.log2("getAmountAndContracts", value);
          AmountAndContracts(value) |> Future.value;
        }
      | _ => TotalInfoFailed |> Future.value,
    );

let getFundraiserStats = (~timestamp, ~config) =>
  ApiCall.getForQueryApi(
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
      | Some(value) => {
          Js.log2("getFundraiserStats", value);
          FundraiserCount(value) |> Future.value;
        }
      | _ => TotalInfoFailed |> Future.value,
    );

let getFundraiserActivated = (~config) =>
  ApiCall.getForQueryApi(
    ~query=Queries.getQueryForTotalFundraiserActivated(),
    ~field="operations",
    ~config,
  )
  ->Future.map(
      fun
      | Some(value) =>
        value |> json_of_magic |> parseFundraiserActivated |> toOption
      | None => None,
    )
  ->Future.flatMap(
      fun
      | Some(value) => {
          Js.log2("getFundraiserActivated", value);
          TotalFundraiserCount(value) |> Future.value;
        }
      | _ => TotalInfoFailed |> Future.value,
    );

let getSumFeeAndGas = (~timestamp, ~config) =>
  ApiCall.getForQueryApi(
    ~query=Queries.getQueryForFeesStats(timestamp),
    ~field="operations",
    ~config,
  )
  ->Future.map(
      fun
      | Some(value) => value |> json_of_magic |> parseSumFeeAndGas |> toOption
      | None => None,
    )
  ->Future.flatMap(
      fun
      | Some((sumFee, sumGas)) => {
          Js.log2(sumFee, sumGas);
          SumFeeAndGas(sumFee, sumGas) |> Future.value;
        }
      | _ => TotalInfoFailed |> Future.value,
    );

let reduceTotalsInfo =
  List.fold_left(
    (accumulative, current) => {
      Js.log2(accumulative, current);
      switch (current) {
      | TotalInfoFailed => accumulative
      | AmountAndContracts(value) =>
        Js.log(value);
        let value = {
          ...accumulative,
          countAmount: value.countAmount,
          sumAmount: value.sumAmount,
          countOriginatedContracts: value.countOriginatedContracts,
        };
        value;
      | FundraiserCount(fundraiserCount) =>
        Js.log(fundraiserCount);
        let value = {...accumulative, fundraiserCount};
        value;
      | TotalFundraiserCount(totalFundraiserCount) =>
        Js.log(totalFundraiserCount);
        let value = {...accumulative, totalFundraiserCount};
        value;
      | SumFeeAndGas(sumFee, sumConsumedGas) =>
        Js.log2(sumFee, sumConsumedGas);
        let value = {...accumulative, sumFee, sumConsumedGas};
        value;
      };
    },
    initTotalsInfo,
  );

let getBlockInfoThunk =
    (~callback, ~metaCycle: int, ~timestamp: float, ~config: MainType.config) =>
  Future.all([getNumBlocks(~metaCycle, ~timestamp, ~config)])
  ->Future.map(reduceBlockInfo)
  ->Future.get(callback);

let getTotalsInfoThunk =
    (~callback, ~timestamp: float, ~config: MainType.config) =>
  Future.all([
    getAmountAndContracts(~timestamp, ~config),
    getFundraiserStats(~timestamp, ~config),
    getFundraiserActivated(~config),
    getSumFeeAndGas(~timestamp, ~config),
  ])
  ->Future.map(reduceTotalsInfo)
  ->Future.get(callback);