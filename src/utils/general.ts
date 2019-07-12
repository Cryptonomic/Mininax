import { ConseilQueryBuilder, ConseilOperator, ConseilFunction, ConseilSortDirection} from 'conseiljs';

const fields = {
  block: [
    {name: 'hash', displayName: 'Hash'},
    {name: 'predecessor', displayName: 'Predecessor'},
    {name: 'level', displayName: 'Level'},
    {name: 'timestamp', displayName: 'Timestamp'},
    {name: 'chain_id', displayName: 'Chain ID'},
    {name: 'protocol', displayName: 'Protocol'},
    {name: 'consumed_gas', displayName: 'Consumed gas'},
    {name: 'total_amount', displayName: 'Total transacted'},
    {name: 'fee', displayName: 'Total Fees'},
    {name: 'baker', displayName: 'Baker'},
    {name: 'baker_priority', displayName: 'Baker Priority'},
    {name: 'meta_cycle', displayName: 'Cycle'},
    {name: 'meta_cycle_position', displayName: 'Cycle Position'},
    {name: 'period_kind', displayName: 'Governance Period'},
    {name: 'active_proposal', displayName: 'Active proposal'},
    {name: 'signature', displayName: 'Signature'}
  ],
  account: [
    {name: 'account_id', displayName: 'Address'},
    {name: 'block_id', displayName: 'Last Updated'},
    {name: 'balance', displayName: 'Balance'},
    {name: 'manager', displayName: 'Manager'},
    {name: 'script', displayName: 'Script'},
    {name: 'storage', displayName: 'Storage'},
    {name: 'baker_deactivated', displayName: 'Active Baker?'},
    {name: 'baker_balance', displayName: 'Snapshot balance'},
    {name: 'baker_delegated_balance', displayName: 'Delegated balance'},
    {name: 'baker_frozen_balance', displayName: 'Frozen balance'},
    {name: 'baker_staking_balance', displayName: 'Staking balance'},
  ],
  operation: {
    transaction: [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
      {name: 'kind', displayName: 'Kind'},
      {name: 'block_hash', displayName: 'Block Hash'},
      {name: 'source', displayName: 'From'},
      {name: 'destination', displayName: 'To'},
      {name: 'amount', displayName: 'Amount'},
      {name: 'fee', displayName: 'Fee'},
      {name: 'consumed_gas', displayName: 'Gas Consumed'},
      {name: 'parameters', displayName: 'Parameter'},
      {name: 'status', displayName: 'Status'}
    ],
    'activate_account': [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
      {name: 'kind', displayName: 'Kind'},
      {name: 'block_hash', displayName: 'Block Hash'},
      {name: 'pkh', displayName: 'Account ID'},
      {name: 'secret', displayName: 'Secret'}
    ],
    reveal: [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
      {name: 'kind', displayName: 'Kind'},
      {name: 'block_hash', displayName: 'Block Hash'},
      {name: 'source', displayName: 'Account ID'},
      {name: 'public_key', displayName: 'Public Key'},
      {name: 'fee', displayName: 'Fee'},
      {name: 'consumed_gas', displayName: 'Gas Consumed'},
      {name: 'status', displayName: 'Status'}
    ],
    delegation: [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
      {name: 'kind', displayName: 'Kind'},
      {name: 'block_hash', displayName: 'Block Hash'},
      {name: 'source', displayName: 'Account ID'},
      {name: 'delegate', displayName: 'Delegate'},
      {name: 'fee', displayName: 'Fee'},
      {name: 'consumed_gas', displayName: 'Gas Consumed'},
      {name: 'status', displayName: 'Status'}
    ],
    origination: [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
      {name: 'kind', displayName: 'Kind'},
      {name: 'block_hash', displayName: 'Block Hash'},
      {name: 'source', displayName: 'Account ID'},
      {name: 'delegate', displayName: 'Delegate'},
      {name: 'amount', displayName: 'Amount'},
      {name: 'fee', displayName: 'Fee'},
      {name: 'consumed_gas', displayName: 'Gas Consumed'},
      {name: 'script', displayName: 'Script'},
      {name: 'storage', displayName: 'Storage'},
      {name: 'status', displayName: 'Status'}
    ]
  }
};

export function getQueryForOperations(operationid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addPredicate(query, 'operation_group_hash', ConseilOperator.EQ, [operationid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.IN, ['transaction', 'activate_account', 'reveal', 'delegation', 'origination'], false);
  return query;
}

export function getQueryForBlockTotals(blockid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'block_hash', 'amount', 'fee');
  query = ConseilQueryBuilder.addPredicate(query, 'block_hash', ConseilOperator.EQ, [blockid], false);
  query = ConseilQueryBuilder.addAggregationFunction(query, 'fee', ConseilFunction.sum);
  query = ConseilQueryBuilder.addAggregationFunction(query, 'amount', ConseilFunction.sum);
  return query;
}

export function getQueryForBlockLink(blockid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'block_hash', 'operation_group_hash', 'kind', 'source', 'destination', 'amount', 'fee', 'slots');
  query = ConseilQueryBuilder.addPredicate(query, 'block_hash', ConseilOperator.EQ, [blockid], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForBakerInfo(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'deactivated', 'balance', 'delegated_balance', 'staking_balance', 'frozen_balance');
  query = ConseilQueryBuilder.addPredicate(query, 'pkh', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addOrdering(query, 'block_level', ConseilSortDirection.DESC);
  query = ConseilQueryBuilder.setLimit(query, 1);
  return query;
}

export function getQueryForAccountSends(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'timestamp', 'block_hash', 'operation_group_hash', 'kind', 'source', 'destination', 'amount', 'fee', 'status');
  query = ConseilQueryBuilder.addPredicate(query, 'source', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.EQ, ['transaction'], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForAccountReceipts(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'timestamp', 'block_hash', 'operation_group_hash', 'kind', 'source', 'destination', 'amount', 'fee', 'status');
  query = ConseilQueryBuilder.addPredicate(query, 'destination', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.EQ, ['transaction'], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForOtherOperations(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'timestamp', 'block_hash', 'operation_group_hash', 'source', 'kind', 'status');
  query = ConseilQueryBuilder.addPredicate(query, 'source', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.IN, ['reveal', 'delegation', 'origination'], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForEndorsements(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'timestamp', 'block_hash', 'block_level', 'operation_group_hash', 'kind', 'delegate', 'slots');
  query = ConseilQueryBuilder.addPredicate(query, 'delegate', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.EQ, ['endorsement'], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForBakedBlocks(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'timestamp', 'hash', 'level', 'baker');
  query = ConseilQueryBuilder.addPredicate(query, 'baker', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function getQueryForDepositsAndRewards(accountid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'source_hash', 'delegate', 'category', 'change');
  query = ConseilQueryBuilder.addPredicate(query, 'delegate', ConseilOperator.EQ, [accountid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'source', ConseilOperator.EQ, ['block'], false);
  query = ConseilQueryBuilder.setLimit(query, 1000);
  return query;
}

export function convertFromUtezToTez(amountInUtez: number) {
  const tezAmount = amountInUtez / 1000000;
  return `${tezAmount} XTZ`;
}

export function getFields(key: string, kind?: string) {
  if (!key) {
    return [];
  }
  if (key !== 'operation') {
    return fields[key];
  }

  if (fields[key][kind]) {
    return fields[key][kind];
  }

  return [];
}

export function copyContent(val: string) {
  const textField = document.createElement('textarea');
  textField.innerText = val;
  document.body.appendChild(textField);
  textField.select();
  document.execCommand('copy');
  textField.remove();
}
