open MainType;

let initState = {
  inited: 0,
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
    sumAmount: 0,
  },
  blockinfo: {
    blockCount: 0,
    fundraiserCount: "0",
    totalFundraiserCount: "0",
    sum_fee: 0,
    sum_consumed_gas: 0,
    num_bakers: "0",
    bakers_sum_staking_balance: 0.0,
    totalTez: 0.0,
  },
  voteinfo: {
    yay_rolls: 0,
    nay_rolls: 0,
    pass_rolls: 0,
    proposal_hash: "",
    current_expected_quorum: 0,
  },
  proposals: [||],
};

let globalReducer = (state, action) =>
  switch (action) {
  | Init(selectedConfig) => {...state, inited: 1, selectedConfig}
  | SetId(id) => {...state, id}
  | SetLoading => {...state, isLoading: true}
  | ChangeNetwork(config) => {
      ...state,
      selectedConfig: config,
      isOpenNetworkSelector: false,
    }
  | SetParams(entity, id) => {...state, entity, id}
  | SetError(error) => {...state, error, isError: true, isLoading: false}
  | RemoveError => {...state, error: "", isError: false, isLoading: false}
  | SetBlock(block, id, isLoading) => {
      ...state,
      block,
      entity: "blocks",
      id,
      isLoading,
    }
  | SetAccount(account, id) => {
      ...state,
      account,
      entity: "accounts",
      id,
      isLoading: false,
    }
  | SetOperations(operation, id) => {
      ...state,
      operation,
      entity: "operations",
      id,
      isLoading: false,
    }
  | OpenNetwork(status) => {...state, isOpenNetworkSelector: status}
  | SetLastBlock(lastBlock, blockinfo, transinfo) => {
      ...state,
      lastBlock,
      transinfo,
      blockinfo,
      isLoading: false,
    }
  | SetProposals(proposals) => {...state, proposals}
  | SetVoteInfo(voteinfo) => {...state, voteinfo}
  };

let globalStore =
  Reductive.Store.create(
    ~reducer=globalReducer,
    ~preloadedState=initState,
    (),
  );

module AppStore = {
  include ReductiveContext.Make({
    type action = MainType.action;
    type state = MainType.state;
  });
};