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
  valueColor: string
};

type transInfo = {
  countOriginatedContracts: string,
  countAmount: string,
  sumAmount: int
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
};

let initState = {
  entity: "",
  id: "",
  isLoading: false,
  selectedConfig: 0,
  isOpenNetworkSelector: false,
  isError: false,
  error: "",
  block: Js.Dict.empty(),
  account: Js.Dict.empty(),
  operation: [||],
  lastBlock: Js.Obj.empty(), 
  transinfo: {
    countOriginatedContracts: "0",
    countAmount: "0",
    sumAmount: 0
  }, 
  blockinfo: {
    blockCount: 0,
    fundraiserCount: "0",
    totalFundraiserCount: "0",
    sum_fee: 0,
    sum_consumed_gas: 0, 
    num_bakers: "0", 
    bakers_sum_staking_balance: 0.0, 
    totalTez: 0.0
  }
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
  | SetLastBlock(ConseiljsType.tezosBlock, blockInfo, transInfo);
