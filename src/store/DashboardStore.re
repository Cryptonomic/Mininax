// type transInfo = {
//   countOriginatedContracts: string,
//   countAmount: string,
//   sumAmount: int,
// };

// type voteInfo = {
//   yay_rolls: int,
//   nay_rolls: int,
//   pass_rolls: int,
//   proposal_hash: string,
//   current_expected_quorum: int,
// };

// type blockInfo = {
//   blockCount: int,
//   fundraiserCount: string,
//   totalFundraiserCount: string,
//   sum_fee: int,
//   sum_consumed_gas: int,
//   num_bakers: string,
//   bakers_sum_staking_balance: float,
//   totalTez: float,
// };

type proposalInfo = {
  count_operation_group_hash: int,
  proposal: option(string),
};

type blocksInfo = {blockCount: option(int)};
type blockInfoChunk =
  | BlockCount(int)
  | BlockInfoFailed;
let initBlockInfo = {blockCount: None};

type amountAndContracts = {
  countAmount: option(int),
  sumAmount: option(int),
  countOriginatedContracts: option(int),
};

type totalsInfo = {
  countAmount: option(int),
  sumAmount: option(int),
  countOriginatedContracts: option(int),
  fundraiserCount: option(int),
  sumFee: option(int),
  sumConsumedGas: option(int),
  totalFundraiserCount: option(int),
};
type totalsInfoChunk =
  | AmountAndContracts(amountAndContracts)
  | FundraiserCount(option(int))
  | SumFeeAndGas(option(int), option(int))
  | TotalFundraiserCount(option(int))
  | TotalInfoFailed;
let initTotalsInfo = {
  countAmount: None,
  sumAmount: None,
  countOriginatedContracts: None,
  fundraiserCount: None,
  sumFee: None,
  sumConsumedGas: None,
  totalFundraiserCount: None,
};

type governanceProcessInfo = {
  yayRolls: option(int),
  nayRolls: option(int),
  passRolls: option(int),
  proposalHash: option(string),
  currentExpectedQuorum: option(int),
};
type voteInfo = {
  yayRolls: option(int),
  nayRolls: option(int),
  passRolls: option(int),
  proposalHash: option(string),
};
type governanceProcessInfoChunk =
  | VoteInfo(voteInfo)
  | CurrentExpectedQuorum(option(int))
  | GovernanceProcessInfoFailed;
let initGovernanceProcessInfo = {
  yayRolls: None,
  nayRolls: None,
  passRolls: None,
  proposalHash: None,
  currentExpectedQuorum: None,
};

type bakersInfo = {
  bakersSumStakingBalance: option(float),
  totalTez: option(float),
};
type bakersInfoChunk =
  | BakersSumStakingBalance(option(float))
  | TotalTez(option(float))
  | BakersInfoFailed;
let intBakersInfo = {bakersSumStakingBalance: None, totalTez: None};

type state = {
  loading: bool,
  lastBlock: MainType.tezosBlock,
  blocksInfo,
  totalsInfo,
  proposalsInfo: option(array(proposalInfo)),
  governanceProcessInfo,
  bakersInfo,
};

let initState = {
  loading: true,
  lastBlock: MainType.initTezosBlock,
  blocksInfo: initBlockInfo,
  totalsInfo: initTotalsInfo,
  governanceProcessInfo: initGovernanceProcessInfo,
  proposalsInfo: None,
  bakersInfo: intBakersInfo,
};

type baker = {
  countAccountId: string,
  sumBalance: int,
  delegateValue: string,
};

type storageDelta = {
  sumFee: int,
  sumPaid_storage_size_diff: int,
  sumConsumed_gas: int,
  countOperationGroupHash: string,
};

// type action =
//   | SetLastBlock(ConseiljsType.tezosBlock, blockInfo, transInfo)
//   | SetProposals(array(proposalInfo))
//   | SetVoteInfo(voteInfo);

type otherTotals =
  | CountedTransactions24h(int)
  | CountedZeroPriorityBlocksLevels24h(int)
  | CountedBakers24h(int)
  | CountOriginationAndReveal(option(int), option(int))
  | GetTop3Bakers(array(baker))
  | GetStorageDelta24h(storageDelta);

type action =
  | SetLastBlock(MainType.tezosBlock)
  | SetBlockInfo(blocksInfo)
  | SetTotalsInfo(totalsInfo)
  | SetGovernanceProcess(governanceProcessInfo)
  | SetBackerInfo(bakersInfo)
  | SetProposalInfo(option(array(proposalInfo)))
  | Loaded;

let reducer = (state, action) =>
  switch (action) {
  | SetLastBlock(lastBlock) => {...state, lastBlock}
  | SetBlockInfo(blocksInfo) => {...state, blocksInfo}
  | SetTotalsInfo(totalsInfo) => {...state, totalsInfo}
  | SetGovernanceProcess(governanceProcessInfo) => {
      ...state,
      governanceProcessInfo,
    }
  | SetBackerInfo(bakersInfo) => {...state, bakersInfo}
  | SetProposalInfo(proposalsInfo) => {...state, proposalsInfo}
  | Loaded => {...state, loading: false}
  };