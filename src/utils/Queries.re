/*
   The main problem in this file is code repetition.
   If we examine all functions they have a very similar pattern:
   1. create an empty query
   2. execute ConseilQueryBuilder function where a query is always passed as the first parameter
   return last ConseilQueryBuilder result
   Also, other similarities:
   - all functions produce a blank query
   - addFields always accept the same type: list(string)
   - addPredicate always use query as the first argument,  ConseiljsType variant as third, and false bool as last
   - addAggregationFunction always use query as first argument a field name as second and SUM or COUNT variant as last

   What we are going to do:
     Pipe all data through ConseilQueryBuilder methods to simplify code
     At the moment there are a lot of variables declaration and shadowing (query variable redeclaration).
     We declare variables only to pass them as parameters. They are extra text that makes code a bit less readable.
     Also thanks to pipe operators: pipe-last `|>` & pipe-frist `->` we are able to simplify functions body in that way
     let simple_fun = param => createEmptyQuery -> operation1 -> operation 2 -> operation3;
     It let us focus on how we are going to process our data instead of folowing which variables went where.
    Why are we going to do that?
    It will make code cleaner and help to focus on processing data instead of redeclaring variables.

   Important:
   - Pipes:
   More idiomatic for ReasonML (from Ocaml perspective) is `|>` pipe-last. It applies value as the last argument to a piped function
   Havely used by BuckleScript is `->` pipe-first. It applies value as the first argument to piped function
   While handling BuckleScript related code we should use `->` pipe, in other custom functions we rather should use `|>` pipe last. In our case because unlabeled ConseiljsRe API we going to use `->` more.
   - Labels:
   It's good practice and highly recommended to label positional parameters and JS binding. Labels are not only important as descriptive part of code
   While ConseiljsRe doesn't provide a function with labels we have to deal with the partial application a bit different way. See QueriesPlus.re file for more details
 */
open ConseiljsRe;
// while this module mostly reuse ConseilQueryBuilder functionality it would save a lot of space opening module here
open ConseiljsRe.ConseilQueryBuilder;

let oneDayMillis = 86400000.0;
let getTimeYesterday = (nowTime: float) => nowTime -. oneDayMillis;

// by removing all variables from function and defining flow via pipe (piper first in this example) we can achive much cleaner code
// isn't it easier to understand what happening with query, is it?
let getQueryForBlockTotals = (blockid: string) =>
  // We're creating query
  blankQuery()
  // adding fields to it
  ->addFields(["block_hash", "amount", "fee"])
  // adding predicate to field
  ->addPredicate("block_hash", ConseiljsType.EQ, `Str([|blockid|]), false)
  // adinng aggregation to field
  ->addAggregationFunction("fee", ConseiljsType.SUM)
  // ading aggregation to another field
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