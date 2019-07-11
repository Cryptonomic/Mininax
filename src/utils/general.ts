import { ConseilQueryBuilder, ConseilOperator, ConseilFunction} from 'conseiljs';

const fields = {
  block: [
    {name: 'hash', displayName: 'Hash'},
    {name: 'predecessor', displayName: 'Predecessor'},
    {name: 'level', displayName: 'Level'},
    {name: 'timestamp', displayName: 'TimeStamp'},
    {name: 'chain_id', displayName: 'Chain ID'},
    {name: 'protocol', displayName: 'Protocol'},
    {name: 'consumed_gas', displayName: 'Consumed gas'},
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
  ],
  operation: {
    default: [
      {name: 'operation_group_hash', displayName: 'Operation Hash'},
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
      {name: 'secret', displayName: 'Secret'},
      {name: 'status', displayName: 'Status'}
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
    ]
  }
};

export function getQueryForOperations(operationid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addPredicate(query, 'operation_group_hash', ConseilOperator.EQ, [operationid], false);
  query = ConseilQueryBuilder.addPredicate(query, 'kind', ConseilOperator.IN, ['transaction', 'activate_account', 'reveal', 'delegation', 'origination'], false);
  return query;
}

export function getQueryForBlockFee(blockid: string) {
  let query = ConseilQueryBuilder.blankQuery();
  query = ConseilQueryBuilder.addFields(query, 'block_hash', 'fee');
  query = ConseilQueryBuilder.addPredicate(query, 'block_hash', ConseilOperator.EQ, [blockid], false);
  query = ConseilQueryBuilder.addAggregationFunction(query, 'fee', ConseilFunction.sum);
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

  return fields[key]['default'];
}

export function copyContent(val: string) {
  const textField = document.createElement('textarea');
  textField.innerText = val;
  document.body.appendChild(textField);
  textField.select();
  document.execCommand('copy');
  textField.remove();
}
