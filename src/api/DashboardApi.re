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
      | Some(value) => AmountAndContracts(value) |> Future.value
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
      | Some(value) => FundraiserCount(value) |> Future.value
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
      | Some(value) => TotalFundraiserCount(value) |> Future.value
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
      | Some((sumFee, sumGas)) =>
        SumFeeAndGas(sumFee, sumGas) |> Future.value
      | _ => TotalInfoFailed |> Future.value,
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
      | SumFeeAndGas(sumFee, sumConsumedGas) => {
          ...accumulative,
          sumFee,
          sumConsumedGas,
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

let getQuorumInfo = (~hash: string, ~config: MainType.config) =>
  ApiCall.getForQueryApi(
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
  ApiCall.getForQueryApi(
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

let getProposalsInfoThunk =
    (~callback, ~metaCycle: int, ~config: MainType.config) =>
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
    )
  ->Future.get(callback);

let getGovernanceProcessInfoThunk =
    (
      ~callback,
      ~hash: string,
      ~active_proposal: string,
      ~config: MainType.config,
    ) =>
  Future.all([
    getQuorumInfo(~hash, ~config),
    getVoteInfo(~hash, ~active_proposal, ~config),
  ])
  ->Future.map(reduceGevernanceInfo)
  ->Future.get(callback);