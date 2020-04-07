open ConseiljsRe;
// while this module mostly reuse ConseilQueryBuilder functionality it would save a lot of space opening module here
open ConseiljsRe.ConseilQueryBuilder;

let oneDayMillis = 86400000.0;
let getTimeYesterday = (nowTime: float) => nowTime -. oneDayMillis;

let getQueryForBlockTotals = (blockid: string) =>
  blankQuery()
  ->addFields(["block_hash", "amount", "fee"])
  ->addPredicate("block_hash", ConseiljsType.EQ, `Str([|blockid|]), false)
  ->addAggregationFunction("fee", ConseiljsType.SUM)
  ->addAggregationFunction("amount", ConseiljsType.SUM);

let getQueryForOperations = (operationid: string) =>
  blankQuery()
  ->addPredicate(
      "operation_group_hash",
      ConseiljsType.EQ,
      `Str([|operationid|]),
      false,
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
  ->addPredicate("pkh", ConseiljsType.EQ, `Str([|accountid|]), false)
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
  ->addPredicate("block_hash", ConseiljsType.EQ, `Str([|id|]), false)
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
  ->addPredicate("source", ConseiljsType.EQ, `Str([|id|]), false)
  ->addPredicate("kind", ConseiljsType.EQ, `Str([|"transaction"|]), false)
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
  ->addPredicate("destination", ConseiljsType.EQ, `Str([|id|]), false)
  ->addPredicate("kind", ConseiljsType.EQ, `Str([|"transaction"|]), false)
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
  ->addPredicate("source", ConseiljsType.EQ, `Str([|id|]), false)
  ->addPredicate(
      "kind",
      ConseiljsType.IN,
      `Str([|"reveal", "delegation", "origination"|]),
      false,
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
  ->addPredicate("delegate", ConseiljsType.EQ, `Str([|id|]), false)
  ->addPredicate("kind", ConseiljsType.EQ, `Str([|"endorsement"|]), false)
  ->setLimit(1000);

let getQueryForBakedBlocks = (id: string) =>
  blankQuery()
  ->addFields(["timestamp", "hash", "level", "baker"])
  ->addPredicate("baker", ConseiljsType.EQ, `Str([|id|]), false)
  ->setLimit(1000);

let getQueryForDepositsAndRewards = (id: string) =>
  blankQuery()
  ->addFields(["source_hash", "delegate", "category", "change"])
  ->addPredicate("delegate", ConseiljsType.EQ, `Str([|id|]), false)
  ->addPredicate("source", ConseiljsType.EQ, `Str([|"block"|]), false)
  ->setLimit(1000);

let getQueryForNumBlocks = (metaCycle: int, timeStamp: float) =>
  blankQuery()
  ->addFields(["hash"])
  ->addPredicate("meta_cycle", ConseiljsType.EQ, `Int([|metaCycle|]), false)
  ->addPredicate(
      "timestamp",
      ConseiljsType.BEFORE,
      `Float([|timeStamp|]),
      false,
    )
  ->ConseilQueryBuilder.addAggregationFunction("hash", ConseiljsType.COUNT);

let getQueryForTransactionStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["amount", "originated_contracts"])
  ->addPredicate(
      "kind",
      ConseiljsType.IN,
      `Str([|"transaction", "origination"|]),
      false,
    )
  ->addPredicate("status", ConseiljsType.EQ, `Str([|"applied"|]), false)
  ->addPredicate(
      "timestamp",
      ConseiljsType.AFTER,
      `Float([|getTimeYesterday(timeStamp)|]),
      false,
    )
  ->addPredicate(
      "timestamp",
      ConseiljsType.BEFORE,
      `Float([|timeStamp|]),
      false,
    )
  ->addAggregationFunction("originated_contracts", ConseiljsType.COUNT)
  ->addAggregationFunction("amount", ConseiljsType.COUNT)
  ->addAggregationFunction("amount", ConseiljsType.SUM);

let getQueryForFundraiserStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["kind"])
  ->addPredicate(
      "kind",
      ConseiljsType.EQ,
      `Str([|"activate_account"|]),
      false,
    )
  ->addPredicate(
      "timestamp",
      ConseiljsType.AFTER,
      `Float([|getTimeYesterday(timeStamp)|]),
      false,
    )
  ->addPredicate(
      "timestamp",
      ConseiljsType.BEFORE,
      `Float([|timeStamp|]),
      false,
    )
  ->addAggregationFunction("kind", ConseiljsType.COUNT);

let getQueryForTotalFundraiserActivated = () =>
  blankQuery()
  ->addFields(["kind"])
  ->addPredicate(
      "kind",
      ConseiljsType.EQ,
      `Str([|"activate_account"|]),
      false,
    )
  ->addAggregationFunction("kind", ConseiljsType.COUNT);

let getQueryForFeesStats = (timeStamp: float) =>
  blankQuery()
  ->addFields(["fee", "consumed_gas"])
  ->addPredicate(
      "timestamp",
      ConseiljsType.AFTER,
      `Float([|getTimeYesterday(timeStamp)|]),
      false,
    )
  ->addPredicate(
      "timestamp",
      ConseiljsType.BEFORE,
      `Float([|timeStamp|]),
      false,
    )
  ->addAggregationFunction("fee", ConseiljsType.SUM)
  ->addAggregationFunction("consumed_gas", ConseiljsType.SUM);

let getQueryForBakerStats = () =>
  blankQuery()
  ->addFields(["pkh", "staking_balance"])
  ->addPredicate("deactivated", ConseiljsType.EQ, `Str([|"false"|]), false)
  ->addAggregationFunction("pkh", ConseiljsType.COUNT)
  ->addAggregationFunction("staking_balance", ConseiljsType.SUM);

let getQueryForMarketCap = () =>
  blankQuery()
  ->addFields(["balance"])
  ->addAggregationFunction("balance", ConseiljsType.SUM);

let getQueryForProposalInfo = (cycle: int) =>
  blankQuery()
  ->addFields(["proposal", "operation_group_hash"])
  ->addPredicate("cycle", ConseiljsType.EQ, `Int([|cycle|]), false)
  ->addPredicate("proposal", ConseiljsType.ISNULL, `Str([|""|]), true)
  ->addAggregationFunction("operation_group_hash", ConseiljsType.COUNT);

let getQueryForTestingInfo = (cycle: int) =>
  blankQuery()
  ->addFields(["proposal_hash"])
  ->addPredicate("cycle", ConseiljsType.EQ, `Int([|cycle|]), false);

let getQueryForQuorum = (hash: string) =>
  blankQuery()
  ->addFields(["current_expected_quorum"])
  ->addPredicate("hash", ConseiljsType.EQ, `Str([|hash|]), false);

let getQueryForVotingStats = (hash: string, propasal: string) =>
  blankQuery()
  ->addFields(["yay_rolls", "nay_rolls", "pass_rolls", "proposal_hash"])
  ->addPredicate("block_hash", ConseiljsType.EQ, `Str([|hash|]), false)
  ->addPredicate(
      "proposal_hash",
      ConseiljsType.EQ,
      `Str([|propasal|]),
      false,
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