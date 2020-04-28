type transInfo = {
  countOriginatedContracts: string,
  countAmount: string,
  sumAmount: int,
};

type voteInfo = {
  yay_rolls: int,
  nay_rolls: int,
  pass_rolls: int,
  proposal_hash: string,
  current_expected_quorum: int,
};

type blockInfo = {
  blockCount: int,
  fundraiserCount: string,
  totalFundraiserCount: string,
  sum_fee: int,
  sum_consumed_gas: int,
  num_bakers: string,
  bakers_sum_staking_balance: float,
  totalTez: float,
};

type proposalInfo = {
  count_operation_group_hash: string,
  proposal: string,
};

type state = {
  lastBlock: ConseiljsType.tezosBlock,
  transinfo: transInfo,
  blockinfo: blockInfo,
  voteinfo: voteInfo,
  proposalsInfo: array(proposalInfo),
};

// type action =
//   | SetBlockInfo(int)
//   | SetTotalsInfo(int)
//   | SetGovernanceProcess(int)
//   | SetBackerInfo(int);

let initState = {
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
  proposalsInfo: [||],
};

type action =
  | SetLastBlock(ConseiljsType.tezosBlock, blockInfo, transInfo)
  | SetProposals(array(proposalInfo))
  | SetVoteInfo(voteInfo);

let reducer = (state, action) =>
  switch (action) {
  // | SetBlockInfo(int) => store
  // | SetTotalsInfo(int) => store
  // | SetGovernanceProcess(int) => store
  // | SetBackerInfo(int) => store
  //////////////////////////////////////////
  | SetLastBlock(lastBlock, blockinfo, transinfo) => {
      ...state,
      lastBlock,
      transinfo,
      blockinfo,
    }
  | SetProposals(proposalsInfo) => {...state, proposalsInfo}
  | SetVoteInfo(voteinfo) => {...state, voteinfo}
  };