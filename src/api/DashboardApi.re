open DashboardStore;
open Decode;
open Helpers;

module Fold = {
  let reduceBlockInfo =
    List.fold_left(
      (accumulative, current) =>
        switch (current) {
        | BlockInfoFailed => accumulative
        | BlockCount(value) => {blockCount: Some(value)}
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
        | SumFeeAndGas(sumFee, sumConsumedGas) => {
            ...accumulative,
            sumFee,
            sumConsumedGas,
          }
        | CountedTransactions(countTransactions) => {
            ...accumulative,
            countTransactions,
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
        | TotalTez(totalTez) => {...accumulative, totalTez}
        }
      },
      intBakersInfo,
    );
};

module Calls = {
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
        | Some(value) =>
          value |> json_of_magic |> parseSumFeeAndGas |> toOption
        | None => None,
      )
    ->Future.flatMap(
        fun
        | Some((sumFee, sumGas)) =>
          SumFeeAndGas(sumFee, sumGas) |> Future.value
        | _ => TotalInfoFailed |> Future.value,
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

  let getSumStakingBalance = (~config) =>
    ApiCall.getForQueryApi(
      ~query=Queries.getQueryForBakerStats(),
      ~field="delegates",
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
    ApiCall.getForQueryApi(
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
};

module Thunk = {
  open Calls;
  open Fold;

  let getBlockInfoThunk =
      (
        ~callback,
        ~metaCycle: int,
        ~timestamp: float,
        ~config: MainType.config,
      ) =>
    Future.all([getNumBlocks(~metaCycle, ~timestamp, ~config)])
    ->Future.map(reduceBlockInfo)
    ->Future.get(callback);

  let getTotalsInfoThunk =
      (~callback, ~timestamp: float, ~config: MainType.config) => {
    let startDate =
      timestamp
      |> MomentRe.momentWithTimestampMS
      |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
      |> MomentRe.Moment.valueOf;
    Future.all([
      getAmountAndContracts(~timestamp, ~config),
      getFundraiserStats(~timestamp, ~config),
      getFundraiserActivated(~config),
      getSumFeeAndGas(~timestamp, ~config),
      getLastDayTransactions(~startDate, ~endDate=timestamp, ~config),
    ])
    ->Future.map(reduceTotalsInfo)
    ->Future.get(callback);
  };

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

  let getBakersInfoThunk = (~callback, ~config: MainType.config) =>
    Future.all([getSumStakingBalance(~config), getSumTez(~config)])
    ->Future.map(reduceBakersInfo)
    ->Future.get(callback);
};