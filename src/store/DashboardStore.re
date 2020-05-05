type proposalInfo = {
  count_operation_group_hash: int,
  proposal: option(string),
};

type activationOriginationReveal = {
  reveal: option(int),
  contractDeployed: option(int),
  activatedFundraiserCount: option(int),
};
let intActivationOriginationReveal = {
  reveal: None,
  contractDeployed: None,
  activatedFundraiserCount: None,
};

type baker = {
  countAccountId: string,
  sumBalance: int,
  delegateValue: string,
};

type storageDelta = {
  sumFee: int,
  storageDelta: int,
  sumConsumedGas: int,
  countOperationGroupHash: string,
};

type blocksInfo = {
  blockCount: option(int),
  zeroPriorityBlocks: option(int),
  bakersWithOutput: option(int),
};
type blockInfoChunk =
  | BlockCount(int)
  | CountedBakers(option(int))
  | CountedZeroPriorityBlocksLevels(option(int))
  | BlockInfoFailed;
let initBlockInfo = {
  blockCount: None,
  zeroPriorityBlocks: None,
  bakersWithOutput: None,
};

type amountAndContracts = {
  countAmount: option(int),
  sumAmount: option(int),
  countOriginatedContracts: option(int),
};

type totalsInfo = {
  countAmount: option(int),
  sumAmount: option(int),
  countOriginatedContracts: option(int),
  countTransactions: option(int),
  fundraiserCount: option(int),
  sumFee: option(int),
  sumConsumedGas: option(int),
  storageDelta: option(int),
  totalFundraiserCount: option(int),
  reveals: option(int),
  contractDeployed: option(int),
};
type totalsInfoChunk =
  | AmountAndContracts(amountAndContracts)
  | FundraiserCount(option(int))
  | SumFeeGasDelta(storageDelta)
  | TotalFundraiserCount(option(int))
  | CountedTransactions(option(int))
  | ActivationOriginationReveal(activationOriginationReveal)
  | TotalInfoFailed;
let initTotalsInfo = {
  countAmount: None,
  sumAmount: None,
  countOriginatedContracts: None,
  countTransactions: None,
  fundraiserCount: None,
  sumFee: None,
  sumConsumedGas: None,
  storageDelta: None,
  totalFundraiserCount: None,
  reveals: None,
  contractDeployed: None,
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
  top3Bakers: option(array(baker)),
  totalTez: option(float),
};
type bakersInfoChunk =
  | BakersSumStakingBalance(option(float))
  | Top3Bakers(option(array(baker)))
  | TotalTez(option(float))
  | BakersInfoFailed;
let intBakersInfo = {
  bakersSumStakingBalance: None,
  top3Bakers: None,
  totalTez: None,
};

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