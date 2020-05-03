type config = {
  url: string,
  apiKey: string,
  platform: string,
  network: string,
  displayName: string,
};

type field = {
  name: string,
  displayName: string,
  isLink: bool,
  showNotifierLink: bool,
};

type conseilServerInfo = {
  .
  "url": string,
  "apiKey": string,
  "network": string,
};

type theme = {
  mainBgColor: string,
  fieldBgColor: string,
  searchBgColor: string,
  fieldColor: string,
  valueColor: string,
  fontCol1: string,
  fontCol2: string,
  levelBgcol: string,
  networkBgCol: string,
  networkShadowCol: string,
};

type transactionHash = {countedTransactions: int};

type tezosBlock = {
  active_proposal: option(string),
  baker: option(string),
  chain_id: option(string),
  consumed_gas: option(int),
  context: option(string),
  current_expected_quorum: option(int),
  expected_commitment: option(bool),
  fitness: option(string),
  hash: option(string),
  level: option(int),
  meta_cycle: option(int),
  meta_cycle_position: option(int),
  meta_level: option(int),
  meta_level_position: option(int),
  meta_voting_period: option(int),
  meta_voting_period_position: option(int),
  nonce_hash: option(string),
  operations_hash: option(string),
  period_kind: option(string),
  predecessor: option(string),
  priority: option(int),
  proto: option(int),
  protocol: option(string),
  signature: option(string),
  timestamp: option(float),
  utc_day: option(int),
  utc_month: option(int),
  utc_time: option(string),
  utc_year: option(int),
  validation_pass: option(int),
};

let initTezosBlock = {
  active_proposal: None,
  baker: None,
  chain_id: None,
  consumed_gas: None,
  context: None,
  current_expected_quorum: None,
  expected_commitment: None,
  fitness: None,
  hash: None,
  level: None,
  meta_cycle: None,
  meta_cycle_position: None,
  meta_level: None,
  meta_level_position: None,
  meta_voting_period: None,
  meta_voting_period_position: None,
  nonce_hash: None,
  operations_hash: None,
  period_kind: None,
  predecessor: None,
  priority: None,
  proto: None,
  protocol: None,
  signature: None,
  timestamp: None,
  utc_day: None,
  utc_month: None,
  utc_time: None,
  utc_year: None,
  validation_pass: None,
};