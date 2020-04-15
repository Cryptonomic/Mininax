type config = {
  url: string,
  apiKey: string,
  platform: string,
  network: string,
  displayName: string
};

type field = {
  name: string,
  displayName: string,
  isLink: bool,
  showNotifierLink: bool
};

type conseilServerInfo = Js.t({
  . url: string,
  apiKey: string,
  network: string
});

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
  networkShadowCol: string
};

type transInfo = {
  countOriginatedContracts: string,
  countAmount: string,
  sumAmount: int
}

type voteInfo = {
  yay_rolls: int,
  nay_rolls: int,
  pass_rolls: int,
  proposal_hash: string, 
  current_expected_quorum: int
}

type blockInfo = {
  blockCount: int,
  fundraiserCount: string,
  totalFundraiserCount: string,
  sum_fee: int,
  sum_consumed_gas: int, 
  num_bakers: string,
  bakers_sum_staking_balance: float,
  totalTez: float
}

type proposalInfo = {
  count_operation_group_hash: string,
  proposal: string
};

type state = {
  entity: string,
  id: string,
  isLoading: bool,
  selectedConfig: int,
  isOpenNetworkSelector: bool,
  isError: bool,
  error: string,
  block: Js.Dict.t(string),
  account: Js.Dict.t(string),
  operation: array(Js.Dict.t(string)), 
  lastBlock: ConseiljsType.tezosBlock,
  transinfo: transInfo,
  blockinfo: blockInfo,
  voteinfo: voteInfo,
  proposals: array(proposalInfo)
};

type action =
  | SetId(string)
  | SetLoading
  | ChangeNetwork(int)
  | SetParams(string, string)
  | SetError(string)
  | RemoveError
  | SetBlock(Js.Dict.t(string), string, bool)
  | SetAccount(Js.Dict.t(string), string)
  | SetOperations(array(Js.Dict.t(string)), string)
  | OpenNetwork(bool)
  | SetLastBlock(ConseiljsType.tezosBlock, blockInfo, transInfo)
  | SetProposals(array(proposalInfo))
  | SetVoteInfo(voteInfo);
