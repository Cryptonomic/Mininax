open ConseiljsRe;

let oneDayMillis = 86400000.0;
let getTimeYesterday = (nowTime: float) => nowTime -. oneDayMillis;
let getQueryForBlockTotals = (blockid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["block_hash", "amount", "fee"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "block_hash", ConseiljsType.EQ, `Str([|blockid|]), false);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "fee", ConseiljsType.SUM);
  ConseilQueryBuilder.addAggregationFunction(query, "amount", ConseiljsType.SUM);
};

let getQueryForOperations = (operationid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let query = ConseilQueryBuilder.addPredicate(query, "operation_group_hash", ConseiljsType.EQ, `Str([|operationid|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForBakerInfo = (accountid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["deactivated", "balance", "delegated_balance", "staking_balance", "frozen_balance"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "pkh", ConseiljsType.EQ, `Str([|accountid|]), false);
  let query = ConseilQueryBuilder.addOrdering(query, "block_level", ConseiljsType.DESC);
  ConseilQueryBuilder.setLimit(query, 1);
};

let getQueryForBlockLink = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "slots"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "block_hash", ConseiljsType.EQ, `Str([|id|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForAccountSends = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "status"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, `Str([|id|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, `Str([|"transaction"|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForAccountReceipts = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "status"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "destination", ConseiljsType.EQ, `Str([|id|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, `Str([|"transaction"|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForOtherOperations = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["timestamp", "block_hash", "block_level", "operation_group_hash", "source", "kind", "status", "originated_contracts"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, `Str([|id|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.IN, `Str([|"reveal", "delegation", "origination"|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForEndorsements = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["timestamp", "block_hash", "block_level", "operation_group_hash", "kind", "delegate", "slots"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "delegate", ConseiljsType.EQ, `Str([|id|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, `Str([|"endorsement"|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForBakedBlocks = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["timestamp", "hash", "level", "baker"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "baker", ConseiljsType.EQ, `Str([|id|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForDepositsAndRewards = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["source_hash", "delegate", "category", "change"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "delegate", ConseiljsType.EQ, `Str([|id|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, `Str([|"block"|]), false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForNumBlocks = (metaCycle: int, timeStamp: float) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["hash"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "meta_cycle", ConseiljsType.EQ, `Int([|metaCycle|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.BEFORE, `Float([|timeStamp|]), false);
  ConseilQueryBuilder.addAggregationFunction(query, "hash", ConseiljsType.COUNT);
};

let getQueryForTransactionStats = (timeStamp: float) => {
  let timeYesterday = getTimeYesterday(timeStamp);
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["amount", "originated_contracts"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.IN, `Str([|"transaction", "origination"|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.AFTER, `Float([|timeYesterday|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.BEFORE, `Float([|timeStamp|]), false);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "originated_contracts", ConseiljsType.COUNT);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "amount", ConseiljsType.COUNT);
  ConseilQueryBuilder.addAggregationFunction(query, "amount", ConseiljsType.SUM);
};

let getQueryForFundraiserStats = (timeStamp: float) => {
  let timeYesterday = getTimeYesterday(timeStamp);
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["kind"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, `Str([|"activate_account"|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.AFTER, `Float([|timeYesterday|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.BEFORE, `Float([|timeStamp|]), false);
  ConseilQueryBuilder.addAggregationFunction(query, "kind", ConseiljsType.COUNT);
};

let getQueryForTotalFundraiserActivated = () => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["kind"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, `Str([|"activate_account"|]), false);
  ConseilQueryBuilder.addAggregationFunction(query, "kind", ConseiljsType.COUNT);
};

let getQueryForFeesStats = (timeStamp: float) => {
  let timeYesterday = getTimeYesterday(timeStamp);
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["fee", "consumed_gas"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.AFTER, `Float([|timeYesterday|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "timestamp", ConseiljsType.BEFORE, `Float([|timeStamp|]), false);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "fee", ConseiljsType.SUM);
  ConseilQueryBuilder.addAggregationFunction(query, "consumed_gas", ConseiljsType.SUM);
};

let getQueryForBakerStats = () => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["pkh", "staking_balance"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "deactivated", ConseiljsType.EQ, `Str([|"false"|]), false);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "pkh", ConseiljsType.COUNT);
  ConseilQueryBuilder.addAggregationFunction(query, "staking_balance", ConseiljsType.SUM);
};

let getQueryForMarketCap = () => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["balance"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  ConseilQueryBuilder.addAggregationFunction(query, "balance", ConseiljsType.SUM);
};

let getQueryForProposalInfo = (cycle: int) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["proposal", "operation_group_hash"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  let query = ConseilQueryBuilder.addPredicate(query, "cycle", ConseiljsType.EQ, `Int([|cycle|]), false);
  let query = ConseilQueryBuilder.addPredicate(query, "proposal", ConseiljsType.ISNULL, `Str([|""|]), false);
  ConseilQueryBuilder.addAggregationFunction(query, "operation_group_hash", ConseiljsType.COUNT);
};

let getQueryForTestingInfo = (cycle: int) => {
  let query = ConseilQueryBuilder.blankQuery();
  let attributes = ["proposal_hash"];
  let query = ConseilQueryBuilder.addFields(query, attributes);
  ConseilQueryBuilder.addPredicate(query, "cycle", ConseiljsType.EQ, `Int([|cycle|]), false);
};