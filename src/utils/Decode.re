open DashboardStore;

let json_of_magic = magic => magic |> Obj.magic |> Js.Json.object_;

let countedTransactions = json =>
  json
  |> Json.Decode.field("count_operation_group_hash", Json.Decode.string)
  |> int_of_string;

let countedZeroPriorityBlocksLevels = json =>
  json
  |> Json.Decode.field("count_level", Json.Decode.string)
  |> int_of_string;

let countedBaker = json =>
  json
  |> Json.Decode.field("count_baker", Json.Decode.string)
  |> int_of_string;

type originationKind =
  | Reveal
  | Origination
  | ActivateAccount
  | NoKind;

type originationStatus =
  | Applied
  | Backtracked
  | NoStatus;

type originationAndReveals = {
  countOperation: int,
  kind: originationKind,
  status: originationStatus,
};

let countOriginationsAndReveals = json =>
  Json.Decode.{
    countOperation:
      json |> field("count_operation_group_hash", string) |> int_of_string,
    kind:
      json
      |> field("kind", string)
      |> (
        fun
        | "reveal" => Reveal
        | "origination" => Origination
        | "activate_account" => ActivateAccount
        | _ => NoKind
      ),
    status:
      json
      |> field("status", nullable(string))
      |> Js.nullToOption
      |> (
        fun
        | Some("applied") => Applied
        | Some("backtracked") => Backtracked
        | _ => NoStatus
      ),
  };

let parseBaker = json =>
  Json.Decode.{
    countAccountId: json |> field("count_account_id", string),
    sumBalance: json |> field("sum_balance", int),
    delegateValue: json |> field("delegate_value", string),
  };

let getStorageDelta = json =>
  Json.Decode.{
    sumFee: json |> field("sum_fee", int),
    storageDelta: json |> field("sum_paid_storage_size_diff", int),
    sumConsumedGas: json |> field("sum_consumed_gas", int),
    countOperationGroupHash:
      json |> field("count_operation_group_hash", string),
  };

let parseBlockCount = json =>
  json |> Json.Decode.field("count_hash", Json.Decode.string) |> int_of_string;

let parseLatestBlock = (json): MainType.tezosBlock =>
  Json.Decode.{
    active_proposal:
      json |> field("active_proposal", nullable(string)) |> Js.nullToOption,
    baker: json |> field("baker", nullable(string)) |> Js.nullToOption,
    chain_id: json |> field("chain_id", nullable(string)) |> Js.nullToOption,
    consumed_gas:
      json |> field("consumed_gas", nullable(int)) |> Js.nullToOption,
    context: json |> field("context", nullable(string)) |> Js.nullToOption,
    current_expected_quorum:
      json
      |> field("current_expected_quorum", nullable(int))
      |> Js.nullToOption,
    expected_commitment:
      json |> field("expected_commitment", nullable(bool)) |> Js.nullToOption,
    fitness: json |> field("fitness", nullable(string)) |> Js.nullToOption,
    hash: json |> field("hash", nullable(string)) |> Js.nullToOption,
    level: json |> field("level", nullable(int)) |> Js.nullToOption,
    meta_cycle:
      json |> field("meta_cycle", nullable(int)) |> Js.nullToOption,
    meta_cycle_position:
      json |> field("meta_cycle_position", nullable(int)) |> Js.nullToOption,
    meta_level:
      json |> field("meta_level", nullable(int)) |> Js.nullToOption,
    meta_level_position:
      json |> field("meta_level_position", nullable(int)) |> Js.nullToOption,
    meta_voting_period:
      json |> field("meta_voting_period", nullable(int)) |> Js.nullToOption,
    meta_voting_period_position:
      json
      |> field("meta_voting_period_position", nullable(int))
      |> Js.nullToOption,
    nonce_hash:
      json |> field("nonce_hash", nullable(string)) |> Js.nullToOption,
    operations_hash:
      json |> field("operations_hash", nullable(string)) |> Js.nullToOption,
    period_kind:
      json |> field("period_kind", nullable(string)) |> Js.nullToOption,
    predecessor:
      json |> field("predecessor", nullable(string)) |> Js.nullToOption,
    priority: json |> field("priority", nullable(int)) |> Js.nullToOption,
    proto: json |> field("proto", nullable(int)) |> Js.nullToOption,
    protocol: json |> field("protocol", nullable(string)) |> Js.nullToOption,
    signature:
      json |> field("signature", nullable(string)) |> Js.nullToOption,
    timestamp:
      json
      |> field("timestamp", nullable(Json.Decode.float))
      |> Js.nullToOption,
    utc_day: json |> field("utc_day", nullable(int)) |> Js.nullToOption,
    utc_month: json |> field("utc_month", nullable(int)) |> Js.nullToOption,
    utc_time: json |> field("utc_time", nullable(string)) |> Js.nullToOption,
    utc_year: json |> field("utc_year", nullable(int)) |> Js.nullToOption,
    validation_pass:
      json |> field("validation_pass", nullable(int)) |> Js.nullToOption,
  };

let parseAmountAndContracts = (json): amountAndContracts =>
  Json.Decode.{
    sumAmount: json |> field("sum_amount", int) |> Helpers.toOption,
    countAmount:
      json
      |> field("count_amount", string)
      |> int_of_string
      |> Helpers.toOption,
    countOriginatedContracts:
      json
      |> field("count_originated_contracts", string)
      |> int_of_string
      |> Helpers.toOption,
  };

let parseFundraiserStats = json => {
  json
  |> Json.Decode.field("count_kind", Json.Decode.string)
  |> int_of_string
  |> Helpers.toOption;
};

let parseFundraiserActivated = json =>
  json
  |> Json.Decode.field("count_kind", Json.Decode.string)
  |> int_of_string
  |> Helpers.toOption;

let parseSumFeeAndGas = json =>
  Json.Decode.(
    json |> field("sum_fee", nullable(int)) |> Js.nullToOption,
    json |> field("sum_consumed_gas", nullable(int)) |> Js.nullToOption,
  );

let parseProposalInfo = json =>
  Json.Decode.{
    count_operation_group_hash:
      json |> field("count_operation_group_hash", string) |> int_of_string,
    proposal: json |> field("proposal", nullable(string)) |> Js.nullToOption,
  };

let parseProposalsInfo = json => json |> Json.Decode.array(parseProposalInfo);

// TODO should be tested
let parseQuorumInfo = json =>
  Json.Decode.(
    json |> field("current_expected_quorum", string) |> int_of_string
  );

// TODO should be tested
let parseVoteInfo = (json): voteInfo =>
  Json.Decode.{
    yayRolls: json |> field("yay_rolls", int) |> Helpers.toOption,
    nayRolls: json |> field("nay_rolls", int) |> Helpers.toOption,
    passRolls: json |> field("pass_rolls", int) |> Helpers.toOption,
    proposalHash: json |> field("proposal_hash", string) |> Helpers.toOption,
  };

let parseSumStakingBalance = json =>
  Json.Decode.(
    json
    |> field("sum_staking_balance", int)
    |> float_of_int
    |> Helpers.toOption
  );

let parseSumTez = json =>
  Json.Decode.(
    json |> field("sum_balance", int) |> float_of_int |> Helpers.toOption
  );

let parseLatestGovernance = json =>
  Json.Decode.{
    votingPeriod: json |> field("voting_period", int),
    cycle: json |> field("cycle", int),
    blockHash: json |> field("block_hash", string),
  };