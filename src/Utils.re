open ConseiljsRe;
open Type;

[@bs.val] external btoa : string => string = "window.btoa";
[@bs.val] external encodeURIComponent : string => string = "encodeURIComponent";
[@bs.scope "window"] [@bs.val] external open_ : (string, string) => unit = "open";

let arronaxURL = "https://arronax-beta.cryptonomic.tech/";
let invalidId = "You entered an invalid ID.";
let noBlock = "There is no block for the Level.";
let noAvaialbel = "Not available";

let getInfo = (config: Type.config) => {
  let conseilServerInfo: Type.conseilServerInfo = [%bs.obj {url: config.url, apiKey: config.apiKey}];
  (conseilServerInfo, config.platform, config.network);
};

let getDisplayName = (config: Type.config) => config.displayName;

let convertNumberByIntl: (bool, int, int, float ) => string = [%bs.raw {|
    function (useGrouping, minimumFractionDigits, maximumFractionDigits, value) {
      return new Intl.NumberFormat(window.navigator.languages[0], { style: 'decimal', useGrouping, minimumFractionDigits, maximumFractionDigits }).format(value);
    }
  |}];

let formatNumber = (value, isScale) => {
  let realVal = Js.Nullable.toOption(value);
  switch (realVal, isScale) {
  | (Some(val_), true) => {
    let d = float_of_int(val_) /. 1000000.;
    let (minimumFractionDigits, maximumFractionDigits) =
      if (val_ < 10000) {
        (6, 6);
      } else if (val_ < 100000) {
        (4, 4);
      } else {
        (2, 2);
      };
      convertNumberByIntl(true, minimumFractionDigits, maximumFractionDigits, d);
  }
  | (Some(val_), false) => convertNumberByIntl(false, 0, 1, float_of_int(val_))
  | _ => ""
  };
};

let makeUrl = (network: string, entity: string, id: string) => "/" ++ network ++ "/" ++ entity ++ "/" ++ id;

let getFields = (~entity, ~kind=?, ()): array(Type.field) => {
  switch (entity, kind) {
    | ("operation", Some("transaction")) => [|
        {name: "operation_group_hash", displayName: "Operation Hash", isLink: false},
        {name: "kind", displayName: "Kind", isLink: false},
        {name: "block_hash", displayName: "Block Hash", isLink: true},
        {name: "source", displayName: "From", isLink: false},
        {name: "destination", displayName: "To", isLink: false},
        {name: "amount", displayName: "Amount", isLink: false},
        {name: "fee", displayName: "Fee", isLink: false},
        {name: "consumed_gas", displayName: "Gas Consumed", isLink: false},
        {name: "parameters", displayName: "Parameter", isLink: false},
        {name: "status", displayName: "Status", isLink: false}
      |]
    | ("operation", Some("activate_account")) => [|
        {name: "operation_group_hash", displayName: "Operation Hash", isLink: false},
        {name: "kind", displayName: "Kind", isLink: false},
        {name: "block_hash", displayName: "Block Hash", isLink: true},
        {name: "pkh", displayName: "Account ID", isLink: false},
        {name: "secret", displayName: "Secret", isLink: false}
      |]
    | ("operation", Some("reveal")) => [|
        {name: "operation_group_hash", displayName: "Operation Hash", isLink: false},
        {name: "kind", displayName: "Kind", isLink: false},
        {name: "block_hash", displayName: "Block Hash", isLink: true},
        {name: "source", displayName: "Account ID", isLink: false},
        {name: "public_key", displayName: "Public Key", isLink: false},
        {name: "fee", displayName: "Fee", isLink: false},
        {name: "consumed_gas", displayName: "Gas Consumed", isLink: false},
        {name: "status", displayName: "Status", isLink: false}
      |]
    | ("operation", Some("delegation")) => [|
        {name: "operation_group_hash", displayName: "Operation Hash", isLink: false},
        {name: "kind", displayName: "Kind", isLink: false},
        {name: "block_hash", displayName: "Block Hash", isLink: true},
        {name: "source", displayName: "Account ID", isLink: false},
        {name: "delegate", displayName: "Delegate", isLink: false},
        {name: "fee", displayName: "Fee", isLink: false},
        {name: "consumed_gas", displayName: "Gas Consumed", isLink: false},
        {name: "status", displayName: "Status", isLink: false}
      |]
    | ("operation", Some("origination")) => [|
        {name: "operation_group_hash", displayName: "Operation Hash", isLink: false},
        {name: "kind", displayName: "Kind", isLink: false},
        {name: "block_hash", displayName: "Block Hash", isLink: true},
        {name: "source", displayName: "Account ID", isLink: false},
        {name: "delegate", displayName: "Delegate", isLink: false},
        {name: "amount", displayName: "Amount", isLink: false},
        {name: "fee", displayName: "Fee", isLink: false},
        {name: "consumed_gas", displayName: "Gas Consumed", isLink: false},
        {name: "script", displayName: "Script", isLink: false},
        {name: "storage", displayName: "Storage", isLink: false},
        {name: "status", displayName: "Status", isLink: false}
      |]
    | ("block", None) => [|
        {name: "hash", displayName: "Hash", isLink: false},
        {name: "predecessor", displayName: "Predecessor", isLink: true},
        {name: "level", displayName: "Level", isLink: false},
        {name: "timestamp", displayName: "Timestamp", isLink: false},
        {name: "chain_id", displayName: "Chain ID", isLink: false},
        {name: "protocol", displayName: "Protocol", isLink: false},
        {name: "consumed_gas", displayName: "Consumed gas", isLink: false},
        {name: "total_amount", displayName: "Total transacted", isLink: false},
        {name: "fee", displayName: "Total Fees", isLink: false},
        {name: "baker", displayName: "Baker", isLink: true},
        {name: "baker_priority", displayName: "Baker Priority", isLink: false},
        {name: "meta_cycle", displayName: "Cycle", isLink: false},
        {name: "meta_cycle_position", displayName: "Cycle Position", isLink: false},
        {name: "period_kind", displayName: "Governance Period", isLink: false},
        {name: "active_proposal", displayName: "Active proposal", isLink: false},
        {name: "signature", displayName: "Signature", isLink: false}
      |]
    | ("account", None) => [|
        {name: "account_id", displayName: "Address", isLink: false},
        {name: "block_id", displayName: "Last Updated", isLink: true},
        {name: "balance", displayName: "Balance", isLink: false},
        {name: "manager", displayName: "Manager", isLink: true},
        {name: "script", displayName: "Script", isLink: false},
        {name: "storage", displayName: "Storage", isLink: false},
        {name: "baker_deactivated", displayName: "Active Baker?", isLink: false},
        {name: "baker_balance", displayName: "Snapshot balance", isLink: false},
        {name: "baker_delegated_balance", displayName: "Delegated balance", isLink: false},
        {name: "baker_frozen_balance", displayName: "Frozen balance", isLink: false},
        {name: "baker_staking_balance", displayName: "Staking balance", isLink: false},
      |]
    | _ => [||]
  };
};

let convertBlock = (~block, ~total=?, ()) => {
  let assBlock = Js.Obj.assign (Js.Obj.empty(), block);
  let newBlock = Js.Dict.empty();
  Js.Dict.set(newBlock, "hash", assBlock##hash);
  Js.Dict.set(newBlock, "predecessor", assBlock##predecessor);
  Js.Dict.set(newBlock, "level", assBlock##level |> string_of_int);
  Js.Dict.set(newBlock, "timestamp", assBlock##timestamp |> string_of_int);
  Js.Dict.set(newBlock, "chain_id", assBlock##chain_id);
  Js.Dict.set(newBlock, "protocol", assBlock##protocol);
  Js.Dict.set(newBlock, "consumed_gas", formatNumber(assBlock##consumed_gas, false));
  Js.Dict.set(newBlock, "baker", assBlock##baker);
  Js.Dict.set(newBlock, "baker_priority", "Coming Soon");
  Js.Dict.set(newBlock, "meta_cycle", assBlock##meta_cycle |> string_of_int);
  Js.Dict.set(newBlock, "meta_cycle_position", assBlock##meta_cycle_position |> string_of_int);
  Js.Dict.set(newBlock, "period_kind", assBlock##period_kind);
  Js.Dict.set(newBlock, "active_proposal", assBlock##active_proposal);
  Js.Dict.set(newBlock, "signature", assBlock##signature);
  switch (total) {
  | None => {
    Js.Dict.set(newBlock, "total_amount", "");
    Js.Dict.set(newBlock, "fee", "");
    newBlock;
  }
  | Some(tt) => {
    let assTotal = Js.Obj.assign (Js.Obj.empty(), tt);
    Js.Dict.set(newBlock, "total_amount", formatNumber(assTotal##sum_amount, true));
    Js.Dict.set(newBlock, "fee", formatNumber(assTotal##sum_fee, true));
    newBlock;
  }}
};

let convertOperation = (operation) => {
  let assOp = Js.Obj.assign (Js.Obj.empty(), operation);
  let newOp = Js.Dict.empty();
  Js.Dict.set(newOp, "operation_group_hash", assOp##operation_group_hash);
  Js.Dict.set(newOp, "kind", assOp##kind);
  Js.Dict.set(newOp, "block_hash", assOp##block_hash);
  switch assOp##kind {
    | "transaction" => {
      Js.Dict.set(newOp, "source", assOp##source);
      Js.Dict.set(newOp, "destination", assOp##destination);
      Js.Dict.set(newOp, "amount", formatNumber(assOp##amount, true));
      Js.Dict.set(newOp, "fee", formatNumber(assOp##fee, true));
      Js.Dict.set(newOp, "consumed_gas", formatNumber(assOp##consumed_gas, false));
      Js.Dict.set(newOp, "parameters", assOp##parameters);
      Js.Dict.set(newOp, "status", assOp##status);
      newOp;
    }
    | "activate_account" => {
      Js.Dict.set(newOp, "pkh", assOp##pkh);
      Js.Dict.set(newOp, "secret", assOp##secret);
      newOp;
    }
    | "reveal" => {
      Js.Dict.set(newOp, "source", assOp##source);
      Js.Dict.set(newOp, "public_key", assOp##public_key);
      Js.Dict.set(newOp, "fee", formatNumber(assOp##fee, true));
      Js.Dict.set(newOp, "consumed_gas", formatNumber(assOp##consumed_gas, false));
      Js.Dict.set(newOp, "status", assOp##status);
      newOp;
    }
    | "delegation" => {
      Js.Dict.set(newOp, "source", assOp##source);
      Js.Dict.set(newOp, "delegate", assOp##delegate);
      Js.Dict.set(newOp, "fee", formatNumber(assOp##fee, true));
      Js.Dict.set(newOp, "consumed_gas", formatNumber(assOp##consumed_gas, false));
      Js.Dict.set(newOp, "status", assOp##status);
      newOp;
    }
    | "origination" => {
      Js.Dict.set(newOp, "source", assOp##source);
      Js.Dict.set(newOp, "delegate", assOp##delegate);
      Js.Dict.set(newOp, "amount", formatNumber(assOp##amount, true));
      Js.Dict.set(newOp, "fee", formatNumber(assOp##fee, true));
      Js.Dict.set(newOp, "consumed_gas", formatNumber(assOp##consumed_gas, false));
      Js.Dict.set(newOp, "script", assOp##script);
      Js.Dict.set(newOp, "storage", assOp##storage);
      Js.Dict.set(newOp, "status", assOp##status);
      newOp;
    }
    | _ => newOp
  };
};

let convertAccount = (~account, ~baker=?, ()) => {
  let assAccount = Js.Obj.assign (Js.Obj.empty(), account);
  let newAccount = Js.Dict.empty();
  Js.Dict.set(newAccount, "account_id", assAccount##account_id);
  Js.Dict.set(newAccount, "block_id", assAccount##block_id);
  Js.Dict.set(newAccount, "manager", assAccount##manager);
  Js.Dict.set(newAccount, "script", assAccount##script);
  Js.Dict.set(newAccount, "storage", assAccount##storage);
  Js.Dict.set(newAccount, "balance", formatNumber(assAccount##balance, true));
  switch (baker) {
  | None => {
    Js.Dict.set(newAccount, "baker_deactivated", "No");
    Js.Dict.set(newAccount, "baker_balance", "");
    Js.Dict.set(newAccount, "baker_delegated_balance", "");
    Js.Dict.set(newAccount, "baker_frozen_balance", "");
    Js.Dict.set(newAccount, "baker_staking_balance", "");
    newAccount;
  }
  | Some(tt) => {
    let assBaker = Js.Obj.assign (Js.Obj.empty(), tt);
    Js.Dict.set(newAccount, "baker_deactivated", assBaker##deactivated ? "No" : "Yes");
    Js.Dict.set(newAccount, "baker_balance", formatNumber(assBaker##balance , true));
    Js.Dict.set(newAccount, "baker_delegated_balance", formatNumber(assBaker##delegated_balance, true));
    Js.Dict.set(newAccount, "baker_frozen_balance", formatNumber(assBaker##frozen_balance, true));
    Js.Dict.set(newAccount, "baker_staking_balance", formatNumber(assBaker##staking_balance, true));
    newAccount;
  }}
};


let getQueryForBlockTotals = (blockid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["block_hash", "amount", "fee"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "block_hash", ConseiljsType.EQ, [|blockid|], false);
  let query = ConseilQueryBuilder.addAggregationFunction(query, "fee", ConseiljsType.SUM);
  ConseilQueryBuilder.addAggregationFunction(query, "amount", ConseiljsType.SUM);
};

let getQueryForOperations = (operationid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let query = ConseilQueryBuilder.addPredicate(query, "operation_group_hash", ConseiljsType.EQ, [|operationid|], false);
  ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.IN, [|"transaction", "activate_account", "reveal", "delegation", "origination"|], false);
};

let getQueryForBakerInfo = (accountid: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["deactivated", "balance", "delegated_balance", "staking_balance", "frozen_balance"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "pkh", ConseiljsType.EQ, [|accountid|], false);
  let query = ConseilQueryBuilder.addOrdering(query, "block_level", ConseiljsType.DESC);
  ConseilQueryBuilder.setLimit(query, 1);
};

let getQueryForBlockLink = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "slots"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "block_hash", ConseiljsType.EQ, [|id|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForAccountSends = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "status"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, [|id|], false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, [|"transaction"|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForAccountReceipts = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "operation_group_hash", "kind", "source", "destination", "amount", "fee", "status"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "destination", ConseiljsType.EQ, [|id|], false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, [|"transaction"|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForOtherOperations = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "operation_group_hash", "source", "kind", "status"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, [|id|], false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, [|"reveal", "delegation", "origination"|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForEndorsements = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "block_hash", "block_level", "operation_group_hash", "kind", "delegate", "slots"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "delegate", ConseiljsType.EQ, [|id|], false);
  let query = ConseilQueryBuilder.addPredicate(query, "kind", ConseiljsType.EQ, [|"endorsement"|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForBakedBlocks = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["timestamp", "hash", "level", "baker"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "baker", ConseiljsType.EQ, [|id|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let getQueryForDepositsAndRewards = (id: string) => {
  let query = ConseilQueryBuilder.blankQuery();
  let entities = ["source_hash", "delegate", "category", "change"];
  let query = ConseilQueryBuilder.addFields(query, entities);
  let query = ConseilQueryBuilder.addPredicate(query, "delegate", ConseiljsType.EQ, [|id|], false);
  let query = ConseilQueryBuilder.addPredicate(query, "source", ConseiljsType.EQ, [|"block"|], false);
  ConseilQueryBuilder.setLimit(query, 1000);
};

let isNumber = (id: string) => {
  switch (int_of_string(id)) {
    | exception _ => false
    | _ => true
  };
};

let getValueFromDict = (dict: Js.Dict.t(string), key_: string) => {
  switch (Js.Dict.get(dict, key_)) {
    | None => ""
    | Some(value) => value
  };
};

let copyContent: (string) => unit = [%bs.raw {|
    function (value) {
      const textField = document.createElement("textarea");
      textField.innerText = value;
      document.body.appendChild(textField);
      textField.select();
      document.execCommand("copy");
      textField.remove();
    }
  |}];

let jsonConvertQuery: (ConseiljsType.conseilQuery) => string = [%bs.raw {|
    function (value) {
      return JSON.stringify(value)
    }
  |}];

let openSharedUrl = (query: ConseiljsType.conseilQuery, displayName: string, entity: string) => {
  let serializedQuery = jsonConvertQuery(query);
  let encodedUrl = btoa(serializedQuery);
  let shareLink = arronaxURL ++ "?e=" ++ encodeURIComponent(displayName) ++ "/" ++ encodeURIComponent(entity) ++ "&q=" ++ encodedUrl;
  open_(shareLink, "_blank");
};