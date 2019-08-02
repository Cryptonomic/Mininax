// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ConseiljsRe = require("bs-conseiljs/src/ConseiljsRe.js");
var CamlinternalOO = require("bs-platform/lib/js/camlinternalOO.js");

function getInfo(config) {
  var conseilServerInfo = {
    url: config[/* url */0],
    apiKey: config[/* apiKey */1]
  };
  return /* tuple */[
          conseilServerInfo,
          config[/* platform */2],
          config[/* network */3]
        ];
}

function getDisplayName(config) {
  return config[/* displayName */4];
}

function convertFromUtezToTez(amountInUtez) {
  return amountInUtez / 1000000 | 0;
}

function makeUrl(network, entity, id) {
  return "/" + (network + ("/" + (entity + ("/" + id))));
}

function getFields(entity, kind, param) {
  switch (entity) {
    case "account" : 
        if (kind !== undefined) {
          return /* array */[];
        } else {
          return /* array */[
                  /* record */[
                    /* name */"account_id",
                    /* displayName */"Address",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"block_id",
                    /* displayName */"Last Updated",
                    /* isLink */true
                  ],
                  /* record */[
                    /* name */"balance",
                    /* displayName */"Balance",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"manager",
                    /* displayName */"Manager",
                    /* isLink */true
                  ],
                  /* record */[
                    /* name */"script",
                    /* displayName */"Script",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"storage",
                    /* displayName */"Storage",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker_deactivated",
                    /* displayName */"Active Baker?",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker_balance",
                    /* displayName */"Snapshot balance",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker_delegated_balance",
                    /* displayName */"Delegated balance",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker_frozen_balance",
                    /* displayName */"Frozen balance",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker_staking_balance",
                    /* displayName */"Staking balance",
                    /* isLink */false
                  ]
                ];
        }
    case "block" : 
        if (kind !== undefined) {
          return /* array */[];
        } else {
          return /* array */[
                  /* record */[
                    /* name */"hash",
                    /* displayName */"Hash",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"predecessor",
                    /* displayName */"Predecessor",
                    /* isLink */true
                  ],
                  /* record */[
                    /* name */"level",
                    /* displayName */"Level",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"timestamp",
                    /* displayName */"Timestamp",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"chain_id",
                    /* displayName */"Chain ID",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"protocol",
                    /* displayName */"Protocol",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"consumed_gas",
                    /* displayName */"Consumed gas",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"total_amount",
                    /* displayName */"Total transacted",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"fee",
                    /* displayName */"Total Fees",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"baker",
                    /* displayName */"Baker",
                    /* isLink */true
                  ],
                  /* record */[
                    /* name */"baker_priority",
                    /* displayName */"Baker Priority",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"meta_cycle",
                    /* displayName */"Cycle",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"meta_cycle_position",
                    /* displayName */"Cycle Position",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"period_kind",
                    /* displayName */"Governance Period",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"active_proposal",
                    /* displayName */"Active proposal",
                    /* isLink */false
                  ],
                  /* record */[
                    /* name */"signature",
                    /* displayName */"Signature",
                    /* isLink */false
                  ]
                ];
        }
    case "operation" : 
        if (kind !== undefined) {
          switch (kind) {
            case "activate_account" : 
                return /* array */[
                        /* record */[
                          /* name */"operation_group_hash",
                          /* displayName */"Operation Hash",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"kind",
                          /* displayName */"Kind",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"block_hash",
                          /* displayName */"Block Hash",
                          /* isLink */true
                        ],
                        /* record */[
                          /* name */"pkh",
                          /* displayName */"Account ID",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"secret",
                          /* displayName */"Secret",
                          /* isLink */false
                        ]
                      ];
            case "delegation" : 
                return /* array */[
                        /* record */[
                          /* name */"operation_group_hash",
                          /* displayName */"Operation Hash",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"kind",
                          /* displayName */"Kind",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"block_hash",
                          /* displayName */"Block Hash",
                          /* isLink */true
                        ],
                        /* record */[
                          /* name */"source",
                          /* displayName */"Account ID",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"delegate",
                          /* displayName */"Delegate",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"fee",
                          /* displayName */"Fee",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"consumed_gas",
                          /* displayName */"Gas Consumed",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"status",
                          /* displayName */"Status",
                          /* isLink */false
                        ]
                      ];
            case "origination" : 
                return /* array */[
                        /* record */[
                          /* name */"operation_group_hash",
                          /* displayName */"Operation Hash",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"kind",
                          /* displayName */"Kind",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"block_hash",
                          /* displayName */"Block Hash",
                          /* isLink */true
                        ],
                        /* record */[
                          /* name */"source",
                          /* displayName */"Account ID",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"delegate",
                          /* displayName */"Delegate",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"amount",
                          /* displayName */"Amount",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"fee",
                          /* displayName */"Fee",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"consumed_gas",
                          /* displayName */"Gas Consumed",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"script",
                          /* displayName */"Script",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"storage",
                          /* displayName */"Storage",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"status",
                          /* displayName */"Status",
                          /* isLink */false
                        ]
                      ];
            case "reveal" : 
                return /* array */[
                        /* record */[
                          /* name */"operation_group_hash",
                          /* displayName */"Operation Hash",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"kind",
                          /* displayName */"Kind",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"block_hash",
                          /* displayName */"Block Hash",
                          /* isLink */true
                        ],
                        /* record */[
                          /* name */"source",
                          /* displayName */"Account ID",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"public_key",
                          /* displayName */"Public Key",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"fee",
                          /* displayName */"Fee",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"consumed_gas",
                          /* displayName */"Gas Consumed",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"status",
                          /* displayName */"Status",
                          /* isLink */false
                        ]
                      ];
            case "transaction" : 
                return /* array */[
                        /* record */[
                          /* name */"operation_group_hash",
                          /* displayName */"Operation Hash",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"kind",
                          /* displayName */"Kind",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"block_hash",
                          /* displayName */"Block Hash",
                          /* isLink */true
                        ],
                        /* record */[
                          /* name */"source",
                          /* displayName */"From",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"destination",
                          /* displayName */"To",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"amount",
                          /* displayName */"Amount",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"fee",
                          /* displayName */"Fee",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"consumed_gas",
                          /* displayName */"Gas Consumed",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"parameters",
                          /* displayName */"Parameter",
                          /* isLink */false
                        ],
                        /* record */[
                          /* name */"status",
                          /* displayName */"Status",
                          /* isLink */false
                        ]
                      ];
            default:
              return /* array */[];
          }
        } else {
          return /* array */[];
        }
    default:
      return /* array */[];
  }
}

function convertBlock(block, total, param) {
  var assBlock = Object.assign({ }, block);
  var newBlock = { };
  newBlock["hash"] = assBlock.hash;
  newBlock["predecessor"] = assBlock.predecessor;
  newBlock["level"] = String(assBlock.level);
  newBlock["timestamp"] = String(assBlock.timestamp);
  newBlock["chain_id"] = assBlock.chain_id;
  newBlock["protocol"] = assBlock.protocol;
  newBlock["consumed_gas"] = String(assBlock.consumed_gas);
  newBlock["baker"] = assBlock.baker;
  newBlock["baker_priority"] = "Coming Soon";
  newBlock["meta_cycle"] = String(assBlock.meta_cycle);
  newBlock["meta_cycle_position"] = String(assBlock.meta_cycle_position);
  newBlock["period_kind"] = assBlock.period_kind;
  newBlock["active_proposal"] = assBlock.active_proposal;
  newBlock["signature"] = assBlock.signature;
  if (total !== undefined) {
    var assTotal = Object.assign({ }, Caml_option.valFromOption(total));
    newBlock["total_amount"] = String(assTotal.sum_amount / 1000000 | 0);
    newBlock["fee"] = String(assTotal.sum_fee / 1000000 | 0);
    return newBlock;
  } else {
    newBlock["total_amount"] = "";
    newBlock["fee"] = "";
    return newBlock;
  }
}

function convertOperation(operation) {
  var assOp = Object.assign({ }, operation);
  var newOp = { };
  newOp["operation_group_hash"] = assOp.operation_group_hash;
  newOp["kind"] = assOp.kind;
  newOp["block_hash"] = assOp.block_hash;
  var match = assOp.kind;
  switch (match) {
    case "activate_account" : 
        newOp["pkh"] = assOp.pkh;
        newOp["secret"] = assOp.secret;
        return newOp;
    case "delegation" : 
        newOp["source"] = assOp.source;
        newOp["delegate"] = assOp.delegate;
        newOp["fee"] = String(assOp.fee / 1000000 | 0);
        newOp["consumed_gas"] = String(assOp.consumed_gas / 1000000 | 0);
        newOp["status"] = assOp.status;
        return newOp;
    case "origination" : 
        newOp["source"] = assOp.source;
        newOp["delegate"] = assOp.delegate;
        newOp["amount"] = String(assOp.amount / 1000000 | 0);
        newOp["fee"] = String(assOp.fee / 1000000 | 0);
        newOp["consumed_gas"] = String(assOp.consumed_gas / 1000000 | 0);
        newOp["script"] = assOp.script;
        newOp["storage"] = assOp.storage;
        newOp["status"] = assOp.status;
        return newOp;
    case "reveal" : 
        newOp["source"] = assOp.source;
        newOp["public_key"] = assOp.public_key;
        newOp["fee"] = String(assOp.fee / 1000000 | 0);
        newOp["consumed_gas"] = String(assOp.consumed_gas / 1000000 | 0);
        newOp["status"] = assOp.status;
        return newOp;
    case "transaction" : 
        newOp["source"] = assOp.source;
        newOp["destination"] = assOp.destination;
        newOp["amount"] = String(assOp.amount / 1000000 | 0);
        newOp["fee"] = String(assOp.fee / 1000000 | 0);
        newOp["consumed_gas"] = String(assOp.consumed_gas / 1000000 | 0);
        newOp["parameters"] = assOp.parameters;
        newOp["status"] = assOp.status;
        return newOp;
    default:
      return newOp;
  }
}

function convertAccount(account, baker, param) {
  var assAccount = Object.assign({ }, account);
  var newAccount = { };
  newAccount["account_id"] = assAccount.account_id;
  newAccount["block_id"] = assAccount.block_id;
  newAccount["manager"] = assAccount.manager;
  newAccount["script"] = assAccount.script;
  newAccount["storage"] = assAccount.storage;
  newAccount["balance"] = String(assAccount.balance / 1000000 | 0);
  if (baker !== undefined) {
    var assBaker = Object.assign({ }, Caml_option.valFromOption(baker));
    var match = assBaker.deactivated;
    newAccount["baker_deactivated"] = match ? "No" : "Yes";
    newAccount["baker_balance"] = String(assBaker.balance / 1000000 | 0);
    newAccount["baker_delegated_balance"] = String(assBaker.delegated_balance / 1000000 | 0);
    newAccount["baker_frozen_balance"] = String(assBaker.frozen_balance / 1000000 | 0);
    newAccount["baker_staking_balance"] = String(assBaker.staking_balance / 1000000 | 0);
    return newAccount;
  } else {
    newAccount["baker_deactivated"] = "No";
    newAccount["baker_balance"] = "";
    newAccount["baker_delegated_balance"] = "";
    newAccount["baker_frozen_balance"] = "";
    newAccount["baker_staking_balance"] = "";
    return newAccount;
  }
}

function getQueryForBlockTotals(blockid) {
  var query = ConseiljsRe.ConseilQueryBuilder[/* blankQuery */0](/* () */0);
  var query$1 = ConseiljsRe.ConseilQueryBuilder[/* addFields */3](query, /* :: */[
        "block_hash",
        /* :: */[
          "amount",
          /* :: */[
            "fee",
            /* [] */0
          ]
        ]
      ]);
  var query$2 = ConseiljsRe.ConseilQueryBuilder[/* addPredicate */1](query$1, "block_hash", /* EQ */1, /* array */[blockid], false);
  var query$3 = ConseiljsRe.ConseilQueryBuilder[/* addAggregationFunction */7](query$2, "fee", /* SUM */4);
  return ConseiljsRe.ConseilQueryBuilder[/* addAggregationFunction */7](query$3, "amount", /* SUM */4);
}

function getQueryForOperations(operationid) {
  var query = ConseiljsRe.ConseilQueryBuilder[/* blankQuery */0](/* () */0);
  var query$1 = ConseiljsRe.ConseilQueryBuilder[/* addPredicate */1](query, "operation_group_hash", /* EQ */1, /* array */[operationid], false);
  return ConseiljsRe.ConseilQueryBuilder[/* addPredicate */1](query$1, "kind", /* IN */2, /* array */[
              "transaction",
              "activate_account",
              "reveal",
              "delegation",
              "origination"
            ], false);
}

function getQueryForBakerInfo(accountid) {
  var query = ConseiljsRe.ConseilQueryBuilder[/* blankQuery */0](/* () */0);
  var query$1 = ConseiljsRe.ConseilQueryBuilder[/* addFields */3](query, /* :: */[
        "deactivated",
        /* :: */[
          "balance",
          /* :: */[
            "delegated_balance",
            /* :: */[
              "staking_balance",
              /* :: */[
                "frozen_balance",
                /* [] */0
              ]
            ]
          ]
        ]
      ]);
  var query$2 = ConseiljsRe.ConseilQueryBuilder[/* addPredicate */1](query$1, "pkh", /* EQ */1, /* array */[accountid], false);
  var query$3 = ConseiljsRe.ConseilQueryBuilder[/* addOrdering */4](query$2, "block_level", /* DESC */1);
  return ConseiljsRe.ConseilQueryBuilder[/* setLimit */5](query$3, 1);
}

function getQueryForBlockLink(id) {
  var query = ConseiljsRe.ConseilQueryBuilder[/* blankQuery */0](/* () */0);
  var query$1 = ConseiljsRe.ConseilQueryBuilder[/* addFields */3](query, /* :: */[
        "block_hash",
        /* :: */[
          "operation_group_hash",
          /* :: */[
            "kind",
            /* :: */[
              "source",
              /* :: */[
                "destination",
                /* :: */[
                  "amount",
                  /* :: */[
                    "fee",
                    /* :: */[
                      "slots",
                      /* [] */0
                    ]
                  ]
                ]
              ]
            ]
          ]
        ]
      ]);
  var query$2 = ConseiljsRe.ConseilQueryBuilder[/* addPredicate */1](query$1, "block_hash", /* EQ */1, /* array */[id], false);
  return ConseiljsRe.ConseilQueryBuilder[/* setLimit */5](query$2, 1000);
}

function isNumber(id) {
  try {
    Caml_format.caml_int_of_string(id);
    return true;
  }
  catch (exn){
    return false;
  }
}

var copyContent = (
    function (value) {
      const textField = document.createElement("textarea");
      textField.innerText = value;
      document.body.appendChild(textField);
      textField.select();
      document.execCommand("copy");
      textField.remove();
    }
  );

var class_tables = [
  0,
  0,
  0
];

function openSharedUrl(query, displayName, entity) {
  if (!class_tables[0]) {
    var $$class = CamlinternalOO.create_table(0);
    var env = CamlinternalOO.new_variable($$class, "");
    var env_init = function (env$1) {
      var self = CamlinternalOO.create_object_opt(0, $$class);
      self[env] = env$1;
      return self;
    };
    CamlinternalOO.init_class($$class);
    class_tables[0] = env_init;
  }
  return Curry._1(class_tables[0], 0);
}

var arronaxURL = "https://arronax-beta.cryptonomic.tech/";

var invalidId = "You entered an invalid ID.";

var noBlock = "There is no block for the Level.";

var noAvaialbel = "Not available";

exports.arronaxURL = arronaxURL;
exports.invalidId = invalidId;
exports.noBlock = noBlock;
exports.noAvaialbel = noAvaialbel;
exports.getInfo = getInfo;
exports.getDisplayName = getDisplayName;
exports.convertFromUtezToTez = convertFromUtezToTez;
exports.makeUrl = makeUrl;
exports.getFields = getFields;
exports.convertBlock = convertBlock;
exports.convertOperation = convertOperation;
exports.convertAccount = convertAccount;
exports.getQueryForBlockTotals = getQueryForBlockTotals;
exports.getQueryForOperations = getQueryForOperations;
exports.getQueryForBakerInfo = getQueryForBakerInfo;
exports.getQueryForBlockLink = getQueryForBlockLink;
exports.isNumber = isNumber;
exports.copyContent = copyContent;
exports.openSharedUrl = openSharedUrl;
/* copyContent Not a pure module */