type config = {
  url: string,
  apiKey: string,
  platform: string,
  network: string,
  displayName: string,
};

type field = {
  name: string,
  displayName: string,
  isLink: bool,
  showNotifierLink: bool,
};

type conseilServerInfo = {
  .
  "url": string,
  "apiKey": string,
  "network": string,
};

type theme = {
  mainBgColor: string,
  fieldBgColor: string,
  searchBgColor: string,
  fieldColor: string,
  valueColor: string,
  fontCol1: string,
  fontCol2: string,
  levelBgcol: string,
  networkBgCol: string,
  networkShadowCol: string,
};

type transactionHash = {countedTransactions: int};

type otherTotals =
  | CountedTransactions24h(int)
  | CountedZeroPriorityBlocksLevels24h(int)
  | CountedBakers24h(int)
  | CountOriginationAndReveal(option(int), option(int))
  | GetTop3Bakers(array(Decode.baker))
  | GetStorageDelta24h(Decode.storageDelta);