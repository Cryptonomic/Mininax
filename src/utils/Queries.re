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

let labelAddPredicateInverse =
    (
      query: ConseiljsType.conseilQuery,
      ~field: string,
      ~type_: ConseiljsType.conseilOperator,
      ~aggrSetType: ConseiljsType.aggrSetType,
    ) =>
  addPredicate(query, field, type_, aggrSetType, true);

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
      ~field="proposal",
      ~type_=ConseiljsType.ISNULL,
      ~aggrSetType=`Str([|""|]),
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

let getQueryForLastDayTransactions = (startDate: float, endDate: float) =>
  blankQuery()
  ->addFields(["operation_group_hash"])
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"transaction"|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BETWEEN,
      ~aggrSetType=`Float([|startDate, endDate|]),
    )
  ->labelAddAggregationFunction(
      ~field="operation_group_hash",
      ~aggType=ConseiljsType.COUNT,
    )
  ->setLimit(1000);

let getQueryForZeroPriorityBlocksLast24 = (startDate: float, endDate: float) =>
  blankQuery()
  ->addFields(["priority", "level"])
  ->labelAddPredicate(
      ~field="priority",
      ~type_=ConseiljsType.EQ,
      ~aggrSetType=`Str([|"0"|]),
    )
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BETWEEN,
      ~aggrSetType=`Float([|startDate, endDate|]),
    )
  ->labelAddAggregationFunction(~field="level", ~aggType=ConseiljsType.COUNT)
  ->setLimit(1000);

let getQueryForBakersWithOutput = (startDate: float, endDate: float) =>
  blankQuery()
  ->addFields(["level", "baker"])
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BETWEEN,
      ~aggrSetType=`Float([|startDate, endDate|]),
    )
  ->labelAddPredicate(
      ~field="level",
      ~type_=ConseiljsType.GT,
      ~aggrSetType=`Int([|0|]),
    )
  ->labelAddAggregationFunction(~field="level", ~aggType=ConseiljsType.COUNT)
  ->labelAddAggregationFunction(~field="baker", ~aggType=ConseiljsType.COUNT)
  ->addOrdering("count_level", ConseiljsType.DESC)
  ->setLimit(3);

let getQueryForOriginationAndRevealLastDay =
    (startDate: float, endDate: float) =>
  blankQuery()
  ->addFields(["kind", "operation_group_hash", "status"])
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BETWEEN,
      ~aggrSetType=`Float([|startDate, endDate|]),
    )
  ->labelAddPredicate(
      ~field="kind",
      ~type_=ConseiljsType.IN,
      ~aggrSetType=`Str([|"activate_account", "origination", "reveal"|]),
    )
  ->labelAddAggregationFunction(
      ~field="operation_group_hash",
      ~aggType=ConseiljsType.COUNT,
    )
  ->setLimit(1000);

let getQueryForTop3BakersLastDay = () =>
  blankQuery()
  ->addFields(["account_id", "balance", "delegate_value"])
  ->labelAddPredicateInverse(
      ~field="delegate_value",
      ~type_=ConseiljsType.ISNULL,
      ~aggrSetType=`Str([|""|]),
    )
  ->labelAddAggregationFunction(
      ~field="account_id",
      ~aggType=ConseiljsType.COUNT,
    )
  ->labelAddAggregationFunction(~field="balance", ~aggType=ConseiljsType.SUM)
  ->addOrdering("sum_balance", ConseiljsType.DESC)
  ->setLimit(3);

let getQueryForStorageDeltaLastDay = (startDate: float, endDate: float) =>
  blankQuery()
  ->addFields([
      "paid_storage_size_diff",
      "consumed_gas",
      "fee",
      "operation_group_hash",
    ])
  ->labelAddPredicate(
      ~field="timestamp",
      ~type_=ConseiljsType.BETWEEN,
      ~aggrSetType=`Float([|startDate, endDate|]),
    )
  ->labelAddAggregationFunction(~field="fee", ~aggType=ConseiljsType.SUM)
  ->labelAddAggregationFunction(
      ~field="paid_storage_size_diff",
      ~aggType=ConseiljsType.SUM,
    )
  ->labelAddAggregationFunction(
      ~field="consumed_gas",
      ~aggType=ConseiljsType.SUM,
    )
  ->labelAddAggregationFunction(
      ~field="operation_group_hash",
      ~aggType=ConseiljsType.COUNT,
    )
  ->setLimit(1000);

let getQueryForTheLatestGovernance = () =>
  blankQuery()
  ->addFields(["voting_period", "cycle", "block_hash"])
  ->addOrdering("voting_period", ConseiljsType.DESC)
  ->setLimit(1);