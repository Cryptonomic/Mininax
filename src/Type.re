type config = {
  url: string,
  apiKey: string,
  platform: string,
  network: string,
  displayName: string
};

type field = {
  name: string,
  displayName: string,
  isLink: bool
};

type conseilServerInfo = Js.t({
  . url: string,
  apiKey: string
});

type theme = {
  mainBgColor: string,
  fieldBgColor: string,
  searchBgColor: string,
  fieldColor: string,
  valueColor: string
};

type state = {
  entity: string,
  id: string,
  isLoading: bool,
  selectedConfig: int,
  isOpenNetworkSelector: bool,
  isError: bool,
  error: string,
  block: Js.Dict.t(string),
  account: Js.Dict.t(string),
  operation: array(Js.Dict.t(string))
};

let initState = {
  entity: "",
  id: "",
  isLoading: false,
  selectedConfig: 0,
  isOpenNetworkSelector: false,
  isError: false,
  error: "",
  block: Js.Dict.empty(),
  account: Js.Dict.empty(),
  operation: [||]
};

type action =
  | SetId(string)
  | SetLoading
  | ChangeNetwork(int)
  | SetParams(string, string)
  | SetError(string)
  | RemoveError
  | SetBlock(Js.Dict.t(string))
  | SetAccount(Js.Dict.t(string))
  | SetOperations(array(Js.Dict.t(string)))
  | OpenNetwork(bool);
