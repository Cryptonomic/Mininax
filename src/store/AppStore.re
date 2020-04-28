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
  operation: array(Js.Dict.t(string)),
};

type action =
  | SetId(string)
  | SetLoading
  | SetLoaded
  | ChangeNetwork(int)
  | SetParams(string, string)
  | SetError(string)
  | RemoveError
  | SetBlock(Js.Dict.t(string), string, bool)
  | SetAccount(Js.Dict.t(string), string)
  | SetOperations(array(Js.Dict.t(string)), string)
  | OpenNetwork(bool);

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
  operation: [||],
};

let reducer = (state, action) =>
  switch (action) {
  | SetId(id) => {...state, id}
  | SetLoading => {...state, isLoading: true}
  | SetLoaded => {...state, isLoading: false}
  | ChangeNetwork(config) => {
      ...state,
      selectedConfig: config,
      isOpenNetworkSelector: false,
    }
  | SetParams(entity, id) => {...state, entity, id}
  | SetError(error) => {...state, error, isError: true, isLoading: false}
  | RemoveError => {...state, error: "", isError: false, isLoading: false}
  | SetBlock(block, id, isLoading) => {
      ...state,
      block,
      entity: "blocks",
      id,
      isLoading,
    }
  | SetAccount(account, id) => {
      ...state,
      account,
      entity: "accounts",
      id,
      isLoading: false,
    }
  | SetOperations(operation, id) => {
      ...state,
      operation,
      entity: "operations",
      id,
      isLoading: false,
    }
  | OpenNetwork(status) => {...state, isOpenNetworkSelector: status}
  };