/*
    Here I'm going to thing about going even futher eliminating reused code.
    As this changes are optional I would like to present few examples to give you a better view how else it might be optimised, and improved for futher refactor/changes.

   Idea 1
   Defining labeled function to make it easier to understand what we are passing.
   Idea 2
   Defining helper functions
   Cons of this step is we need to create extra functions
   Pros is we can remove all replicated code and store and mantain as separate chunk of code. Important part here is to name helper functions in descriptive way.
   It may sound trivial but it worth to spend a bit more time to choose correct name.

   Problem:
   Js bindings should be labeled to make clear what's you're going to pass to external code.
   Usually we want to label positional parameters and not to label parameter we're going to pipe. In our example we're going to pipe query.
 */

open ConseiljsRe.ConseilQueryBuilder;

module Predicate = {
  //TODO find out what boolean stands for
  // Frist thing we're going to declare labeled function and wrap our oryginall one.
  let labeledAddPredicate =
      (query, ~fieldName, ~conseilType, ~aggrSetType, ~boolean) =>
    addPredicate(query, fieldName, conseilType, aggrSetType, boolean);

  let addPredicateEq =
    labeledAddPredicate(~conseilType=ConseiljsType.EQ, ~boolean=false);
  let addPredicateIn =
    labeledAddPredicate(~conseilType=ConseiljsType.IN, ~boolean=false);
  let addPredicateAfter =
    labeledAddPredicate(~conseilType=ConseiljsType.AFTER, ~boolean=false);
  let addPredicateBefore =
    labeledAddPredicate(~conseilType=ConseiljsType.BEFORE, ~boolean=false);
};

module Aggregation = {
  let labeldAggregationFunction = (query, ~fieldName, ~aggType) =>
    addAggregationFunction(query, fieldName, aggType);
  let addSumAggregationFunctionToField =
    labeldAggregationFunction(~aggType=ConseiljsType.SUM);
  let addCountAggregationFunctionToField =
    labeldAggregationFunction(~aggType=ConseiljsType.COUNT);
};

module Field = {
  let labeledAddField = (query, ~fields: list('a)) =>
    addFields(query, fields);
};

// Now we can use labeled functions to make cleaner what we're passing
let getQueryForBlockTotals_withLabels = (~blockid: string) =>
  blankQuery()
  ->Field.labeledAddField(~fields=["block_hash", "amount", "fee"])
  ->Predicate.labeledAddPredicate(
      ~fieldName="block_hash",
      ~conseilType=ConseiljsType.EQ,
      ~aggrSetType=`Str([|blockid|]),
      ~boolean=false,
    )
  ->Aggregation.labeldAggregationFunction(
      ~fieldName="fee",
      ~aggType=ConseiljsType.SUM,
    )
  ->Aggregation.labeldAggregationFunction(
      ~fieldName="amount",
      ~aggType=ConseiljsType.COUNT,
    );

// We can go even futher, and because Queries.re reuse ConseilQueryBuilder functions with the same parameters a lot, we can use our helper functions.
// It may have sens with insane code repetition, but force us to use helper functions that we have to define and later maintain. Don't take it as 'must have', but it might be helpful in some cases.
let getQueryForBlockTotals_withHelpers = (~blockid: string) =>
  blankQuery()
  ->Field.labeledAddField(~fields=["block_hash", "amount", "fee"])
  ->Predicate.addPredicateEq(
      ~fieldName="block_hash",
      ~aggrSetType=`Str([|blockid|]),
    )
  ->Aggregation.addSumAggregationFunctionToField(~fieldName="fee")
  ->Aggregation.addCountAggregationFunctionToField(~fieldName="amount");