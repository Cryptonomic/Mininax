let arronaxURL = "https://arronax.io/";
type fieldArray = array(MainType.field);

let operationTransactionArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "source", displayName: "From", isLink: true, showNotifierLink: true},
  {
    name: "destination",
    displayName: "To",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "amount",
    displayName: "Amount",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "fee", displayName: "Fee", isLink: false, showNotifierLink: false},
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "parameters",
    displayName: "Parameters",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationActiveAccountArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "pkh",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "secret",
    displayName: "Secret",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationRevealArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "public_key",
    displayName: "Public Key",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "fee", displayName: "Fee", isLink: false, showNotifierLink: false},
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationDelegationArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "delegate",
    displayName: "Delegate",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "fee", displayName: "Fee", isLink: false, showNotifierLink: false},
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationOriginationArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "delegate",
    displayName: "Delegate",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "balance",
    displayName: "Balance",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "fee", displayName: "Fee", isLink: false, showNotifierLink: false},
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "originated_contracts",
    displayName: "Originated Contracts",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "script",
    displayName: "Script",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "storage",
    displayName: "Storage",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationBallotArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "proposal",
    displayName: "Proposal",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "ballot",
    displayName: "Vote",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationSeedNonceRevelationArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "nonce",
    displayName: "Nonce",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationProposalsArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "source",
    displayName: "Account ID",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "proposal",
    displayName: "Proposal",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "consumed_gas",
    displayName: "Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "status",
    displayName: "Status",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationEndorsementArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "delegate",
    displayName: "Delegate",
    isLink: true,
    showNotifierLink: true,
  },
  {
    name: "slots",
    displayName: "Slots",
    isLink: false,
    showNotifierLink: false,
  },
|];

let operationArray: fieldArray = [|
  {
    name: "operation_group_hash",
    displayName: "Operation Hash",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "kind", displayName: "Kind", isLink: false, showNotifierLink: false},
  {
    name: "block_hash",
    displayName: "Block Hash",
    isLink: true,
    showNotifierLink: false,
  },
|];

let blockArray: fieldArray = [|
  {name: "hash", displayName: "Hash", isLink: false, showNotifierLink: false},
  {
    name: "predecessor",
    displayName: "Predecessor",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "level",
    displayName: "Level",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "timestamp",
    displayName: "Timestamp",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "chain_id",
    displayName: "Chain ID",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "protocol",
    displayName: "Protocol",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "consumed_gas",
    displayName: "Consumed Gas",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "total_amount",
    displayName: "Total Transacted",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "fee",
    displayName: "Total Fees",
    isLink: false,
    showNotifierLink: false,
  },
  {name: "baker", displayName: "Baker", isLink: true, showNotifierLink: true},
  {
    name: "baker_priority",
    displayName: "Baker Priority",
    isLink: false,
    showNotifierLink: false,
  },
  /* {name: "meta_cycle", displayName: "Cycle", isLink: false, showNotifierLink: false}, */
  {
    name: "meta_cycle_position",
    displayName: "Cycle Position",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "period_kind",
    displayName: "Governance Period",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "active_proposal",
    displayName: "Active Proposal",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "signature",
    displayName: "Signature",
    isLink: false,
    showNotifierLink: false,
  },
|];

let accountArray: fieldArray = [|
  {
    name: "account_id",
    displayName: "Address",
    isLink: false,
    showNotifierLink: true,
  },
  {
    name: "block_id",
    displayName: "Last Updated",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "balance",
    displayName: "Balance",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "manager",
    displayName: "Manager",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "delegate_value",
    displayName: "Delegate",
    isLink: true,
    showNotifierLink: false,
  },
  {
    name: "script",
    displayName: "Script",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "storage",
    displayName: "Storage",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "baker_deactivated",
    displayName: "Active Baker",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "baker_balance",
    displayName: "Snapshot Balance",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "baker_delegated_balance",
    displayName: "Delegated Balance",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "baker_frozen_balance",
    displayName: "Frozen Balance",
    isLink: false,
    showNotifierLink: false,
  },
  {
    name: "baker_staking_balance",
    displayName: "Staking Balance",
    isLink: false,
    showNotifierLink: false,
  },
|];