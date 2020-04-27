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

type baker = {
  countAccountId: string,
  sumBalance: int,
  delegateValue: string,
};

let getBaker = json =>
  Json.Decode.{
    countAccountId: json |> field("count_account_id", string),
    sumBalance: json |> field("sum_balance", int),
    delegateValue: json |> field("delegate_value", string),
  };

type storageDelta = {
  sumFee: int,
  sumPaid_storage_size_diff: int,
  sumConsumed_gas: int,
  countOperationGroupHash: string,
};

let getStorageDelta = json =>
  Json.Decode.{
    sumFee: json |> field("sum_fee", int),
    sumPaid_storage_size_diff:
      json |> field("sum_paid_storage_size_diff", int),
    sumConsumed_gas: json |> field("sum_consumed_gas", int),
    countOperationGroupHash:
      json |> field("count_operation_group_hash", string),
  };