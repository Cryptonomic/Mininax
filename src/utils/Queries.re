open ConseiljsRe.ConseilQueryBuilder;

let oneDayMillis = 86400000.0;
let getTimeYesterday = (nowTime: float) => nowTime -. oneDayMillis;

let labelAddPredicate =
    (
      query: ConseiljsType.conseilQuery,
      ~field: string,
      ~type_: ConseiljsType.conseilOperator,
      ~aggrSetType: ConseiljsType.aggrSetType,
    ) =>
  addPredicate(query, field, type_, aggrSetType, false);

let labelAddAggregationFunction =
    (
      query: ConseiljsType.conseilQuery,
      ~field: string,
      ~aggType: ConseiljsType.conseilFunction,
    ) =>
  addAggregationFunction(query, field, aggType);

let getQueryForBlockTotals = (blockid: string) =>
  blankQuery()
  ->addFields(["block_hash", "amount", "fee"])
  ->labelAddPredicate(
      ~field="block_hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|blockid|]),
    )
  ->labelAddAggregationFunction(~field="fee", ~aggType=ConseiljsType.SUM)
  ->labelAddAggregationFunction(~field="amount", ~aggType=ConseiljsType.SUM);

let getQueryForOperations = (operationid: string) =>
  blankQuery()
  ->labelAddPredicate(
      ~field="operation_group_hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|operationid|]),
    )
  ->setLimit(1000);

let getQueryForBakerInfo = (accountid: string) =>
  blankQuery()
  ->addFields([
      "deactivated",
      "balance",
      "delegated_balance",
      "staking_balance",
      "frozen_balance",
    ])
  ->labelAddPredicate(
      ~field="pkh",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|accountid|]),
    )
  ->addOrdering("block_level", ConseiljsType.DESC)
  ->setLimit(1);

let getQueryForBlockLink = (id: string) =>
  blankQuery()
  ->addFields([
      "block_hash",
      "operation_group_hash",
      "kind",
      "source",
      "destination",
      "amount",
      "fee",
      "slots",
    ])
  ->labelAddPredicate(
      ~field="block_hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->setLimit(1000);

let getQueryForAccountSends = (id: string) =>
  blankQuery()
  ->addFields([
      "timestamp",
      "block_hash",
      "operation_group_hash",
      "kind",
      "source",
      "destination",
      "amount",
      "fee",
      "status",
    ])
  ->labelAddPredicate(
      ~field="source",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"transaction"|]),
    )
  ->setLimit(1000);

let getQueryForAccountReceipts = (id: string) =>
  blankQuery()
  ->addFields([
      "timestamp",
      "block_hash",
      "operation_group_hash",
      "kind",
      "source",
      "destination",
      "amount",
      "fee",
      "status",
    ])
  ->labelAddPredicate(
      ~field="destination",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"transaction"|]),
    )
  ->setLimit(1000);

let getQueryForOtherOperations = (id: string) =>
  blankQuery()
  ->addFields([
      "timestamp",
      "block_hash",
      "block_level",
      "operation_group_hash",
      "source",
      "kind",
      "status",
      "originated_contracts",
    ])
  ->labelAddPredicate(
      ~field="source",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.IN,
      ~aggrSetType=`Str([|"reveal", "delegation", "origination"|]),
    )
  ->setLimit(1000);

let getQueryForEndorsements = (id: string) =>
  blankQuery()
  ->addFields([
      "timestamp",
      "block_hash",
      "block_level",
      "operation_group_hash",
      "kind",
      "delegate",
      "slots",
    ])
  ->labelAddPredicate(
      ~field="delegate",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"endorsement"|]),
    )
  ->setLimit(1000);

let getQueryForBakedBlocks = (id: string) =>
  blankQuery()
  ->addFields(["timestamp", "hash", "level", "baker"])
  ->labelAddPredicate(
      ~field="baker",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->setLimit(1000);

let getQueryForDepositsAndRewards = (id: string) =>
  blankQuery()
  ->addFields(["source_hash", "delegate", "category", "change"])
  ->labelAddPredicate(
      ~field="delegate",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|id|]),
    )
  ->labelAddPredicate(
      ~field="source",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"block"|]),
    )
  ->setLimit(1000);

let getQueryForNumBlocks = (metaCycle: int, timeStamp: float) =>
  blankQuery()
  ->addFields(["hash"])
  ->labelAddPredicate(
      ~field="meta_cycle",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Int([|metaCycle|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BEFORE,
      ~aggrSetType=`Float([|timeStamp|]),
    )
  ->labelAddAggregationFunction(~field="hash", ~aggType=ConseiljsType.COUNT);

let getQueryForTransactionStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["amount", "originated_contracts"])
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.IN,
      ~aggrSetType=`Str([|"transaction", "origination"|]),
    )
  ->labelAddPredicate(
      ~field="status",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"applied"|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.AFTER,
      ~aggrSetType=`Float([|getTimeYesterday(timeStamp)|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BEFORE,
      ~aggrSetType=`Float([|timeStamp|]),
    )
  ->labelAddAggregationFunction(
      ~field="originated_contracts",
      ~aggType=ConseiljsType.COUNT,
    )
  ->labelAddAggregationFunction(~field="amount", ~aggType=ConseiljsType.COUNT)
  ->labelAddAggregationFunction(~field="amount", ~aggType=ConseiljsType.SUM);

let getQueryForFundraiserStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["kind"])
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"activate_account"|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.AFTER,
      ~aggrSetType=`Float([|getTimeYesterday(timeStamp)|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BEFORE,
      ~aggrSetType=`Float([|timeStamp|]),
    )
  ->labelAddAggregationFunction(~field="kind", ~aggType=ConseiljsType.COUNT);

let getQueryForTotalFundraiserActivated = () =>
  blankQuery()
  ->addFields(["kind"])
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"activate_account"|]),
    )
  ->labelAddAggregationFunction(~field="kind", ~aggType=ConseiljsType.COUNT);

let getQueryForFeesStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["fee", "consumed_gas"])
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.AFTER,
      ~aggrSetType=`Float([|getTimeYesterday(timeStamp)|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BEFORE,
      ~aggrSetType=`Float([|timeStamp|]),
    )
  ->labelAddAggregationFunction(~field="fee", ~aggType=ConseiljsType.SUM)
  ->labelAddAggregationFunction(
      ~field="consumed_gas",
      ~aggType=ConseiljsType.SUM,
    );

let getQueryForBakerStats = () =>
  blankQuery()
  ->addFields(["pkh", "staking_balance"])
  ->labelAddPredicate(
      ~field="deactivated",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"false"|]),
    )
  ->labelAddAggregationFunction(~field="pkh", ~aggType=ConseiljsType.COUNT)
  ->labelAddAggregationFunction(
      ~field="staking_balance",
      ~aggType=ConseiljsType.SUM,
    );

let getQueryForMarketCap = () =>
  blankQuery()
  ->addFields(["balance"])
  ->labelAddAggregationFunction(~field="balance", ~aggType=ConseiljsType.SUM);

let getQueryForProposalInfo = (cycle: int) =>
  blankQuery()
  ->addFields(["proposal", "operation_group_hash"])
  ->labelAddPredicate(
      ~field="cycle",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Int([|cycle|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.ISNULL,
      ~aggrSetType=`Str([|"proposals"|]),
    )
  ->labelAddAggregationFunction(
      ~field="operation_group_hash",
      ~aggType=ConseiljsType.COUNT,
    );

let getQueryForTestingInfo = (cycle: int) =>
  blankQuery()
  ->addFields(["proposal_hash"])
  ->labelAddPredicate(
      ~field="cycle",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Int([|cycle|]),
    );

let getQueryForQuorum = (hash: string) =>
  blankQuery()
  ->addFields(["current_expected_quorum"])
  ->labelAddPredicate(
      ~field="hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|hash|]),
    );

let getQueryForVotingStats = (hash: string, propasal: string) =>
  blankQuery()
  ->addFields(["yay_rolls", "nay_rolls", "pass_rolls", "proposal_hash"])
  ->labelAddPredicate(
      ~field="block_hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|hash|]),
    )
  ->labelAddPredicate(
      ~field="proposal_hash",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|propasal|]),
    );

let getQueryForBlocksTab = () =>
  blankQuery()
  ->addFields([
      "meta_voting_period",
      "meta_cycle",
      "level",
      "timestamp",
      "hash",
      "priority",
    ])
  ->setLimit(1000);
