open Type;
open Configs;
open Themes;

module Styles = {
  open Css;
  let container= index => style([
    width(pct(100.)),
    minHeight(vh(100.)),
    display(flexBox),
    flexDirection(column),
    backgroundColor(hex(themes[index].mainBgColor))
  ]);
  let header = style([padding4(~top=px(45), ~bottom=px(30), ~left=px(45), ~right=px(45))]);
  let headerTitle = style([
    fontFamily("'Arcade', sans-serif"),
    fontSize(px(60)),
    color(white),
    lineHeight(px(35)),
    letterSpacing(px(3)),
    cursor(`pointer),
    display(inlineBlock)
  ]);
  let mainContainer = style([
    padding4(~top=px(0), ~bottom=px(0), ~left=px(35), ~right=px(35)),
    flex(`num(1.)),
    marginBottom(px(40))
  ]);
};

let reducer = (state, action) =>
  switch (action) {
    | SetId(id) => {...state, id}
    | SetLoading => {...state, isLoading: true}
    | ChangeNetwork(config) => {...state, selectedConfig: config, isOpenNetworkSelector: false}
    | SetParams(entity, id) => {...state, entity, id}
    | SetError(error) => {...state, error, isError: true, isLoading: false}
    | RemoveError => {...state, error: "", isError: false, isLoading: false}
    | SetBlock(block, id) => {...state, block, entity: "blocks", id, isLoading: false}
    | SetAccount(account, id) => {...state, account, entity: "accounts", id, isLoading: false}
    | SetOperations(operation, id) => {...state, operation, entity: "operations", id, isLoading: false}
    | OpenNetwork(status) => {...state, isOpenNetworkSelector: status}
  };

let isFirstLoad = ref(false);
let selectedConfig = ref(0);

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let footerRef = ref(None);
  let (state, dispatch) = React.useReducer(reducer, initState);

  let getBlock = (id: string, isRoute: bool, isMain: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getBlockThunk(id, configs[selectedConfig^])
      |> then_(result => {
          switch result {
          | ("Error", Some(error), _) => resolve(dispatch(SetError(error)))
          | ("Valid", _, Some(block)) => {
            switch (isRoute, isMain) {
              | (true, false) => {
                let url = Utils.makeUrl(configs[selectedConfig^].network, "blocks", id);
                ReasonReactRouter.push(url);
                resolve(dispatch(SetBlock(block, id)));
              }
              | (false, false) => resolve(dispatch(SetBlock(block, id)))
              | _ => resolve(dispatch(SetBlock(block, "")))
            };
          }
          | _ => resolve(dispatch(SetError(Utils.noAvaialbel)))
          };
      })
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvaialbel))))
      |> ignore
    );
  };

  let getOperation = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getOperationThunk(id, configs[selectedConfig^])
      |> then_(result => {
          switch result {
          | ("Error", Some(error), _) => resolve(dispatch(SetError(error)))
          | ("Valid", _, Some(operations)) => {
            switch isRoute {
              | true => {
                let url = Utils.makeUrl(configs[selectedConfig^].network, "operations", id);
                ReasonReactRouter.push(url);
                resolve(dispatch(SetOperations(operations, id)));
              }
              | false => resolve(dispatch(SetOperations(operations, id)));
            };
          }
          | _ => resolve(dispatch(SetError(Utils.noAvaialbel)))
          };
      })
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvaialbel))))
      |> ignore
    );
  };

  let getAccount = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getAccountThunk(id, configs[selectedConfig^])
      |> then_(result => {
          switch result {
          | ("Error", Some(error), _) => resolve(dispatch(SetError(error)))
          | ("Valid", _, Some(account)) => {
            switch isRoute {
              | true => {
                let url = Utils.makeUrl(configs[selectedConfig^].network, "accounts", id);
                ReasonReactRouter.push(url);
                resolve(dispatch(SetAccount(account, id)));
              }
              | false => resolve(dispatch(SetAccount(account, id)));
            };
          }
          | _ => resolve(dispatch(SetError(Utils.noAvaialbel)))
          };
      })
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvaialbel))))
      |> ignore
    );
  };

  let getHashByLevel = (level: int) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getBlockHashThunk(level, configs[selectedConfig^])
      |> then_(result =>
        switch (result) {
          | Some(head) => resolve(getBlock(head##hash, true, false))
          | None => resolve(dispatch(SetError(Utils.noAvaialbel)))
        }
      )
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvaialbel))))
      |> ignore
    );
  };

  let getMainPage = () => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getBlockHeadThunk(configs[selectedConfig^])
      |> then_(result =>
        switch (result) {
          | Some(head) => resolve(getBlock(head##hash, false, true))
          | None => resolve(dispatch(SetError(Utils.noAvaialbel)))
        }
      )
      |> ignore
    );
  };

  let goToMainPage = () => {
    getMainPage();
    ReasonReactRouter.push("/");
  };

  let goToPage = (network: string, entity: string, id: string) => {
    let selectedIndex = configs |> Js.Array.findIndex(conf => conf.network === network);
    switch (selectedIndex) {
      | -1 => ReasonReactRouter.push("/")
      | _ => {
        dispatch(ChangeNetwork(selectedIndex));
        selectedConfig := selectedIndex;
        switch entity {
          | "blocks" => getBlock(id, false, false);
          | "accounts" => getAccount(id, false)
          | "operations" => getOperation(id, false)
          | _ => goToMainPage()
        };
      }
    };
  };

  let setFooterRef = (ref) => {
    footerRef := Js.Nullable.toOption(ref);
  };

  let setFocusOfSearch = () => {
    switch (footerRef^) {
      | Some(el) => {
        let _ = Js.Global.setTimeout(_=> {
          let elementObj = ReactDOMRe.domElementToObj(el);
          ignore(elementObj##focus());
          elementObj##select();
        }, 100);
      }
      | None => ignore()
    };
  };
  let onChangeId = (id) => {
    let newId = id |> Js.String.replaceByRe([%re "/ /g"], "");
    dispatch(SetId(newId));
  };

  let onSearchById = (id: string) => {
    let firstChar = id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let twoChars = id |> Js.String.slice(~from=0, ~to_=2) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, twoChars, isNumber) {
      | ("b", _, _) => getBlock(id, true, false)
      | ("o", _, _) => getOperation(id, true)
      | (_, "tz", _) | (_, "kt", _) => getAccount(id, true)
      | (_, _, true) => getHashByLevel(id |> int_of_string)
      | _ => dispatch(SetError(Utils.invalidId))
    };
  };

  let onChangeNetwork = (index: int) => {
    if (state.selectedConfig !== index) {
      dispatch(ChangeNetwork(index));
      selectedConfig := index;
      goToMainPage();
    } else {
      dispatch(OpenNetwork(false));
    }
  };

  if (!isFirstLoad^) {
    isFirstLoad := true;
    switch (url.path) {
      | [network, entity, id] => goToPage(network, entity, id)
      | [] => getMainPage()
      | _ => goToMainPage()
    };
  };

  <ReactIntl.IntlProvider>
    <ContextProvider value={state.selectedConfig}>
      <div
        className=Styles.container(state.selectedConfig)
        tabIndex={0}
        onKeyPress={
          event =>
            switch (event |> ReactEvent.Keyboard.key ) {
            | "s" | "S" => {
              setFocusOfSearch();
            }
            | _ => ignore()
            };
        }
      >
        <div className=Styles.header>
          <div className=Styles.headerTitle>{ReasonReact.string("MININAX")}</div>
        </div>
        <div className=Styles.mainContainer>
          {switch (url.path) {
            | [_, "accounts", _] => <Account items=state.account goToDetail={onSearchById} />
            | [_, "operations", _] => <Operation items=state.operation goToDetail={onSearchById} />
            | _ => <Block items=state.block goToDetail={onSearchById} goToBlock={getHashByLevel} />
          }}
        </div>
        <Footer
          searchVal={state.id}
          network={configs[state.selectedConfig].displayName}
          setRef={setFooterRef}
          changeId={onChangeId}
          onSearch={_ => onSearchById(state.id)}
          onOpenNetworkSelector={_ => dispatch(OpenNetwork(true))}
        />
        {state.isLoading ? <Loader /> : ReasonReact.null}
        {state.isError ? <Error error={state.error} onTry={_ => dispatch(RemoveError)} /> : ReasonReact.null}
        {state.isOpenNetworkSelector ?
          (
            <NetworkSelector
              selectedIndex={state.selectedConfig}
              onChange={onChangeNetwork}
              onCancel={_ => dispatch(OpenNetwork(false))}
            />
          ):ReasonReact.null
        }
      </div>
    </ContextProvider>
  </ReactIntl.IntlProvider>
}
