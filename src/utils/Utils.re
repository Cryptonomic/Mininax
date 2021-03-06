open MainType;
open Constants;
open BsRaw;

open MomentRe;

[@bs.scope "window"] [@bs.val]
external open_: (string, string) => unit = "open";

let getInfo = (config: MainType.config) => {
  let conseilServerInfo: MainType.conseilServerInfo = {
    "url": config.url,
    "apiKey": config.apiKey,
    "network": config.network,
  };
  (conseilServerInfo, config.platform, config.network);
};
let getDisplayName = (config: MainType.config) => config.displayName;
let convertFromUtezToTez = (utez: int) => float_of_int(utez) /. 1000000.;
let convertFromUtezfToTez = (utez: float) => utez /. 1000000.;

let calculateFactionDigits =
  fun
  | x when x < 10000 => (6, 6)
  | x when x < 100000 => (4, 4)
  | _ => (2, 2);

let makeUrl = (~network: string, ~entity: string, ~id: string) =>
  "/" ++ network ++ "/" ++ entity ++ "/" ++ id;

let formatNumber = (value, isScale) => {
  let realVal = Js.Nullable.toOption(value);
  switch (realVal) {
  | Some(val_) when isScale =>
    let (minimumFractionDigits, maximumFractionDigits) =
      calculateFactionDigits(val_);
    convertNumberByIntl(
      true,
      minimumFractionDigits,
      maximumFractionDigits,
      convertFromUtezToTez(val_),
    );
  | Some(val_) => convertNumberByIntl(false, 0, 1, float_of_int(val_))
  | _ => ""
  };
};

let formatString = (value, isConvert) => {
  let realVal = Js.Nullable.toOption(value);
  switch (realVal, isConvert) {
  | (Some(val_), true) =>
    val_
    |> Js.String.split("_")
    |> Array.map(s =>
         Js.String.toUpperCase(Js.String.charAt(0, s))
         ++ Js.String.slice(~from=1, ~to_=Js.String.length(s), s)
       )
    |> Js.Array.joinWith(" ")
  | (Some(val_), false) => val_
  | _ => ""
  };
};

let getFields = (~entity, ~kind=?, ()): array(MainType.field) =>
  switch (entity, kind) {
  | ("operation", Some("Transaction")) => operationTransactionArray
  | ("operation", Some("Activate Account")) => operationActiveAccountArray
  | ("operation", Some("Reveal")) => operationRevealArray
  | ("operation", Some("Delegation")) => operationDelegationArray
  | ("operation", Some("Origination")) => operationOriginationArray
  | ("operation", Some("Ballot")) => operationBallotArray
  | ("operation", Some("Seed Nonce Revelation")) => operationSeedNonceRevelationArray
  | ("operation", Some("Proposals")) => operationProposalsArray
  | ("operation", Some("Endorsement")) => operationEndorsementArray
  | ("operation", _) => operationArray
  | ("block", None) => blockArray
  | ("account", None) => accountArray
  | _ => [||]
  };

let getSecondTimeFromMilli = (nowTime: float) =>
  int_of_float(nowTime /. 1000.);

let isNumber = (id: string) =>
  switch (int_of_string(id)) {
  | exception _ => false
  | _ => true
  };

let getValueFromDict = (dict: Js.Dict.t(string), key_: string) =>
  switch (Js.Dict.get(dict, key_)) {
  | None => ""
  | Some(value) => value
  };

let openSharedUrl =
    (query: ConseiljsType.conseilQuery, displayName: string, entity: string) => {
  let serializedQuery = jsonConvertQuery(query);
  let encodedUrl = btoa(serializedQuery);
  let shareLink =
    arronaxURL
    ++ "?e="
    ++ encodeURIComponent(displayName)
    ++ "/"
    ++ encodeURIComponent(entity)
    ++ "&q="
    ++ encodedUrl;
  open_(shareLink, "_blank");
};

let getBlocksPerCycle = (network: string) =>
  switch (network) {
  | "mainnet" => 4096
  | "babylonnet" => 2048
  | "carthagenet" => 2048
  | "zeronet" => 128
  | _ => 0
  };

let openTelegramBot = (address: string) => {
  let newUrl = "https://t.me/TezosNotifierBot?start=mininax_" ++ address;
  open_(newUrl, "_blank");
};

let getLast24h = () => {
  let startTime =
    momentNow()
    |> Moment.subtract(~duration=duration(1., `days))
    |> Moment.valueOf;
  let endTime = momentNow() |> Moment.valueOf;
  (startTime, endTime);
};

type abberative = {
  symbol: string,
  value: float,
};

let largeNumberabbreviation = (value: float): string =>
  [
    {symbol: "t", value: 1000000000000.},
    {symbol: "B", value: 1000000000.},
    {symbol: "M", value: 1000000.},
    {symbol: "K", value: 1000.},
    {symbol: "", value: 1.},
  ]
  |> List.find(x => x.value < value)
  |> (
    x =>
      x.symbol
      |> (
        value /. x.value |> Js.Float.toFixedWithPrecision(~digits=2) |> (++)
      )
  );