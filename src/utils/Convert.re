open Utils;

let applyProperties = (block, ~field, ~value) => {
  Js.Dict.set(block, field, value);
  block;
};

let convertNoTotal = () =>
  Js.Dict.empty()
  |> applyProperties(~field="total_amount", ~value="")
  |> applyProperties(~field="fee", ~value="");

let convertTotal = (~value) => {
  let assignedTotal = Js.Obj.assign(Js.Obj.empty(), value);
  Js.Dict.empty()
  |> applyProperties(
       ~field="total_amount",
       ~value=formatNumber(assignedTotal##sum_amount, true),
     )
  |> applyProperties(
       ~field="fee",
       ~value=formatNumber(assignedTotal##sum_fee, true),
     );
};

let convertBlock = (~block, ~total=?, ()) => {
  let assignedBlock = Js.Obj.assign(Js.Obj.empty(), block);
  (
    switch (total) {
    | None => convertNoTotal()
    | Some(tt) => convertTotal(~value=tt)
    }
  )
  |> applyProperties(
       ~field="hash",
       ~value=formatString(assignedBlock##hash, false),
     )
  |> applyProperties(
       ~field="hash",
       ~value=formatString(assignedBlock##hash, false),
     )
  |> applyProperties(
       ~field="predecessor",
       ~value=formatString(assignedBlock##predecessor, false),
     )
  |> applyProperties(
       ~field="level",
       ~value=assignedBlock##level |> string_of_int,
     )
  |> applyProperties(
       ~field="meta_voting_period",
       ~value=assignedBlock##meta_voting_period |> string_of_int,
     )
  |> applyProperties(
       ~field="timestamp",
       ~value=assignedBlock##timestamp |> string_of_int,
     )
  |> applyProperties(
       ~field="chain_id",
       ~value=formatString(assignedBlock##chain_id, true),
     )
  |> applyProperties(
       ~field="protocol",
       ~value=formatString(assignedBlock##protocol, false),
     )
  |> applyProperties(
       ~field="consumed_gas",
       ~value=formatNumber(assignedBlock##consumed_gas, false),
     )
  |> applyProperties(
       ~field="baker",
       ~value=formatString(assignedBlock##baker, false),
     )
  |> applyProperties(
       ~field="baker_priority",
       ~value=formatNumber(assignedBlock##priority, false),
     )
  |> applyProperties(
       ~field="meta_cycle",
       ~value=assignedBlock##meta_cycle |> string_of_int,
     )
  |> applyProperties(
       ~field="meta_cycle_position",
       ~value=assignedBlock##meta_cycle_position |> string_of_int,
     )
  |> applyProperties(
       ~field="period_kind",
       ~value=formatString(assignedBlock##period_kind, true),
     )
  |> applyProperties(
       ~field="active_proposal",
       ~value=formatString(assignedBlock##active_proposal, true),
     )
  |> applyProperties(
       ~field="signature",
       ~value=formatString(assignedBlock##signature, false),
     );
};

let convertTransaction = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="source",
       ~value=formatString(value##source, false),
     )
  |> applyProperties(
       ~field="destination",
       ~value=formatString(value##destination, false),
     )
  |> applyProperties(
       ~field="amount",
       ~value=formatNumber(value##amount, true),
     )
  |> applyProperties(~field="fee", ~value=formatNumber(value##fee, true))
  |> applyProperties(
       ~field="consumed_gas",
       ~value=formatNumber(value##consumed_gas, false),
     )
  |> applyProperties(
       ~field="parameters",
       ~value=formatString(value##parameters, false),
     )
  |> applyProperties(
       ~field="status",
       ~value=formatString(value##status, true),
     );

let convertActiveAccount = value =>
  Js.Dict.empty()
  |> applyProperties(~field="pkh", ~value=formatString(value##pkh, false))
  |> applyProperties(
       ~field="secret",
       ~value=formatString(value##secret, false),
     );

let convertBallot = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="proposal",
       ~value=formatString(value##proposal, false),
     )
  |> applyProperties(
       ~field="ballot",
       ~value=formatString(value##ballot, true),
     );
let convertProposals = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="proposal",
       ~value=formatString(value##proposal, false),
     );
let convertSeedNonceRevelation = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="nonce",
       ~value=formatString(value##nonce, false),
     );
let convertReveal = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="source",
       ~value=formatString(value##source, false),
     )
  |> applyProperties(
       ~field="public_key",
       ~value=formatString(value##public_key, false),
     )
  |> applyProperties(~field="fee", ~value=formatNumber(value##fee, true))
  |> applyProperties(
       ~field="consumed_gas",
       ~value=formatNumber(value##consumed_gas, false),
     )
  |> applyProperties(
       ~field="status",
       ~value=formatString(value##status, true),
     );
let convertDelegation = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="source",
       ~value=formatString(value##source, false),
     )
  |> applyProperties(~field="delegate", ~value=value##delegate)
  |> applyProperties(~field="fee", ~value=formatNumber(value##fee, true))
  |> applyProperties(
       ~field="consumed_gas",
       ~value=formatNumber(value##consumed_gas, false),
     )
  |> applyProperties(
       ~field="status",
       ~value=formatString(value##status, true),
     );
let convertOrigination = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="source",
       ~value=formatString(value##source, false),
     )
  |> applyProperties(
       ~field="delegate",
       ~value=formatString(value##delegate, false),
     )
  |> applyProperties(
       ~field="balance",
       ~value=formatNumber(value##balance, true),
     )
  |> applyProperties(~field="fee", ~value=formatNumber(value##fee, true))
  |> applyProperties(
       ~field="consumed_gas",
       ~value=formatNumber(value##consumed_gas, false),
     )
  |> applyProperties(
       ~field="originated_contracts",
       ~value=formatString(value##originated_contracts, false),
     )
  |> applyProperties(
       ~field="script",
       ~value=formatString(value##script, false),
     )
  |> applyProperties(
       ~field="storage",
       ~value=formatString(value##storage, false),
     )
  |> applyProperties(
       ~field="status",
       ~value=formatString(value##status, true),
     );

let convertEndorsement = value =>
  Js.Dict.empty()
  |> applyProperties(
       ~field="delegate",
       ~value=formatString(value##delegate, false),
     )
  |> applyProperties(
       ~field="slots",
       ~value=formatString(value##slots, false),
     );

let convertOperation = operation => {
  let op = Js.Obj.assign(Js.Obj.empty(), operation);
  (
    switch (op##kind) {
    | "transaction" => convertTransaction(op)
    | "activate_account" => convertActiveAccount(op)
    | "ballot" => convertBallot(op)
    | "proposals" => convertProposals(op)
    | "seed_nonce_revelation" => convertSeedNonceRevelation(op)
    | "reveal" => convertReveal(op)
    | "delegation" => convertDelegation(op)
    | "origination" => convertOrigination(op)
    | "endorsement" => convertEndorsement(op)
    | _ => Js.Dict.empty()
    }
  )
  |> applyProperties(
       ~field="operation_group_hash",
       ~value=formatString(op##operation_group_hash, false),
     )
  |> applyProperties(~field="kind", ~value=formatString(op##kind, true))
  |> applyProperties(~field="level", ~value=op##level |> string_of_int)
  |> applyProperties(
       ~field="block_hash",
       ~value=formatString(op##block_hash, false),
     )
  |> applyProperties(
       ~field="timestamp",
       ~value=op##timestamp |> string_of_int,
     );
};

let convertNonBaker = () =>
  Js.Dict.empty()
  |> applyProperties(~field="baker_deactivated", ~value="No")
  |> applyProperties(~field="baker_balance", ~value="")
  |> applyProperties(~field="baker_delegated_balance", ~value="")
  |> applyProperties(~field="baker_frozen_balance", ~value="")
  |> applyProperties(~field="baker_staking_balance", ~value="");

let convertBaker = value => {
  let assignedBaker = Js.Obj.assign(Js.Obj.empty(), value);
  Js.Dict.empty()
  |> applyProperties(
       ~field="baker_deactivated",
       ~value=assignedBaker##deactivated ? "No" : "Yes",
     )
  |> applyProperties(
       ~field="baker_balance",
       ~value=formatNumber(assignedBaker##balance, true),
     )
  |> applyProperties(
       ~field="baker_delegated_balance",
       ~value=formatNumber(assignedBaker##delegated_balance, true),
     )
  |> applyProperties(
       ~field="baker_frozen_balance",
       ~value=formatNumber(assignedBaker##frozen_balance, true),
     )
  |> applyProperties(
       ~field="baker_staking_balance",
       ~value=formatNumber(assignedBaker##staking_balance, true),
     );
};

let convertAccount = (~account, ~baker=?, ()) => {
  let assignedAccount = Js.Obj.assign(Js.Obj.empty(), account);
  (
    switch (baker) {
    | None => convertNonBaker()
    | Some(tt) => convertBaker(tt)
    }
  )
  |> applyProperties(
       ~field="account_id",
       ~value=formatString(assignedAccount##account_id, false),
     )
  |> applyProperties(
       ~field="block_id",
       ~value=formatString(assignedAccount##block_id, false),
     )
  |> applyProperties(
       ~field="manager",
       ~value=formatString(assignedAccount##manager, false),
     )
  |> applyProperties(
       ~field="delegate_value",
       ~value=formatString(assignedAccount##delegate_value, false),
     )
  |> applyProperties(
       ~field="script",
       ~value=formatString(assignedAccount##script, false),
     )
  |> applyProperties(
       ~field="storage",
       ~value=formatString(assignedAccount##storage, false),
     )
  |> applyProperties(
       ~field="balance",
       ~value=formatNumber(assignedAccount##balance, true),
     );
};