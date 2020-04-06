/*
   I would like to discuss how the state is solved here. Overall I think it's okay while the application is small but might be problematic to scale. The same works for context.
   The app file is a good place to implement a router. It gives as a straightforward solution to handle URL changes and clear separation of responsibilities.
 With router we have to separate part of App jsx as separate components and this leads to the problem with state management. While reducer + context is okay, it might be hard to scale. Personally I think that flux tools like reductive (reasonML version of redux) are a great way to handle state changes, implement multiple stores, move part of the logic to actions.
 It's not overkill, even for small applications, but at this point, it would involve a bit more refactoring so I just want to mention the possibility. Thanks to the hooks it allows us to use state everywhere in application easily.

 Examples:
  - moving footer to separate file - cleaner App component
  - moving all api calls to custom hook with actions - most code from App file would be moved to another file
  - defining "routes" dir to handle "smart components" and keep general purpose stateless components in "components" dir - better separation of responsibilites between components

  This futher would allow as to move bigger part of API calls to ApiCall file, extending how we pipe our data. API calls in this file depends on other functions and component state directly.
  We would need to refactor code in a way to pass too many arguments into ApiCall functions. with custom action hook file and global state with flux it would be easier to handle.
  */
open MainType;
open Configs;

module Styles = {
  open Css;
  let container = index =>
    style([
      width(pct(100.)),
      height(pct(100.)),
      minHeight(vh(100.)),
      padding4(~top=px(45), ~bottom=px(45), ~left=px(35), ~right=px(35)),
      backgroundColor(hex(Themes.themes[index].mainBgColor)),
    ]);
  let container1 =
    style([
      width(pct(100.)),
      height(pct(100.)),
      marginLeft(`auto),
      marginRight(`auto),
      maxWidth(px(1510)),
      display(flexBox),
      flexDirection(column),
    ]);
  let header =
    style([
      padding4(~top=px(0), ~bottom=px(30), ~left=px(10), ~right=px(10)),
    ]);
  let headerTitle =
    style([
      fontFamily(`custom("'Arcade', sans-serif")),
      fontSize(px(60)),
      color(white),
      lineHeight(px(35)),
      letterSpacing(px(3)),
      cursor(`pointer),
      display(inlineBlock),
    ]);
  let mainContainer = style([flex(`num(1.)), marginBottom(px(57))]);
};

let reducer = (state, action) =>
  switch (action) {
  | SetId(id) => {...state, id}
  | SetLoading => {...state, isLoading: true}
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
  | SetLastBlock(lastBlock, blockinfo, transinfo) => {
      ...state,
      lastBlock,
      transinfo,
      blockinfo,
      isLoading: false,
    }
  | SetProposals(proposals) => {...state, proposals}
  | SetVoteInfo(voteinfo) => {...state, voteinfo}
  };

// I'm not sure why the ref is even used here
// both mutable variables should be defined as an immutable part of the component state, and change by state-related methods like to set from useState, or changes on reducer state
// selectedConfig is already defined in reducer, maybe it would be better to reuse it or implement another reducer variable if it's required for state transition
let selectedConfig = ref(0);

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let footerRef = ref(None);
  let (state, dispatch) = React.useReducer(reducer, initState);

  let getProposalsInfo = (metaCycle: int) => {
    ApiCall.getProposalInfoThunk(
      ~metaCycle,
      ~config=configs[selectedConfig^],
      ~callback=
        fun
        | Some(proposals) => dispatch(SetProposals(proposals))
        | _ => dispatch(SetError(Utils.noAvailable)),
    );
  };

  let getVoteInfo = (hash: string, active_proposal: string) => {
    Js.Promise.(
      ApiCall.getVoteInfoThunk(
        hash,
        active_proposal,
        configs[selectedConfig^],
      )
      |> then_(result =>
           switch (result) {
           | Some(voteinfo) => resolve(dispatch(SetVoteInfo(voteinfo)))
           | _ => resolve(dispatch(SetError(Utils.noAvailable)))
           }
         )
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvailable))))
      |> ignore
    );
  };

  let getBlockInfo = (block: ConseiljsType.tezosBlock) => {
    Js.Promise.(
      ApiCall.getBlockInfoThunk(
        block##meta_cycle,
        block##timestamp,
        configs[selectedConfig^],
      )
      |> then_(result =>
           switch (result) {
           | (Some(blockinfo), Some(transinfo)) =>
             resolve(dispatch(SetLastBlock(block, blockinfo, transinfo)))
           | _ => resolve(dispatch(SetError(Utils.noAvailable)))
           }
         )
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvailable))))
      |> ignore
    );
  };

  let getBlock = (id: string, isRoute: bool, isMain: bool, level: int) => {
    dispatch(SetLoading);
    let callback = result =>
      switch (result) {
      | Ok((block, lastBlock)) =>
        switch (isRoute, isMain, level) {
        | (true, false, 0) =>
          let url =
            Utils.makeUrl(configs[selectedConfig^].network, "blocks", id);
          ReasonReactRouter.push(url);
          dispatch(SetBlock(block, id, false));
        | (true, false, _) =>
          let strLevel = string_of_int(level);
          let url =
            Utils.makeUrl(
              configs[selectedConfig^].network,
              "blocks",
              strLevel,
            );
          ReasonReactRouter.push(url);
          dispatch(SetBlock(block, strLevel, false));
        | (false, false, _) => dispatch(SetBlock(block, id, false))
        | _ =>
          dispatch(SetBlock(block, "", true));
          switch (lastBlock##period_kind) {
          | "proposal" => getProposalsInfo(lastBlock##meta_cycle)
          | "testing" => ()
          | _ => getVoteInfo(lastBlock##hash, lastBlock##active_proposal)
          };
          getBlockInfo(lastBlock);
        }
      | Error(err) => dispatch(SetError(err))
      };
    ApiCall.getBlockThunk(~callback, ~id, ~config=configs[selectedConfig^]);
  };

  let getOperation = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getOperationThunk(id, configs[selectedConfig^])
      |> then_(result => {
           switch (result) {
           | ("Error", Some(error), _) =>
             resolve(dispatch(SetError(error)))
           | ("Valid", _, Some(operations)) =>
             isRoute
               ? {
                 let url =
                   Utils.makeUrl(
                     configs[selectedConfig^].network,
                     "operations",
                     id,
                   );
                 ReasonReactRouter.push(url);
                 resolve(dispatch(SetOperations(operations, id)));
               }
               : resolve(dispatch(SetOperations(operations, id)))
           | _ => resolve(dispatch(SetError(Utils.noAvailable)))
           }
         })
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvailable))))
      |> ignore
    );
  };

  let getAccount = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getAccountThunk(id, configs[selectedConfig^])
      |> then_(result => {
           switch (result) {
           | ("Error", Some(error), _) =>
             resolve(dispatch(SetError(error)))
           | ("Valid", _, Some(account)) =>
             isRoute
               ? {
                 let url =
                   Utils.makeUrl(
                     configs[selectedConfig^].network,
                     "accounts",
                     id,
                   );
                 ReasonReactRouter.push(url);
                 resolve(dispatch(SetAccount(account, id)));
               }
               : resolve(dispatch(SetAccount(account, id)))
           | _ => resolve(dispatch(SetError(Utils.noAvailable)))
           }
         })
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvailable))))
      |> ignore
    );
  };

  let getHashByLevel = (level: int, isMain: bool) => {
    dispatch(SetLoading);
    Js.Promise.(
      ApiCall.getBlockHashThunk(level, configs[selectedConfig^])
      |> then_(result =>
           switch (result, isMain) {
           | (Some(head), true) =>
             resolve(getBlock(head##hash, false, true, level))
           | (Some(head), false) =>
             resolve(getBlock(head##hash, true, false, level))
           | _ => resolve(dispatch(SetError(Utils.noAvailable)))
           }
         )
      |> catch(_err => resolve(dispatch(SetError(Utils.noAvailable))))
      |> ignore
    );
  };

  let getMainPage = () => {
    dispatch(SetLoading);
    let callback = result =>
      switch (result) {
      | Some(head) => getBlock(head##hash, false, true, 0)
      | None => dispatch(SetError(Utils.noAvailable))
      };
    // After refactoring getBlockHeadThunk we can remove Js.Promise oddity, and pipe callback
    ApiCall.getBlockHeadThunk(~callback, ~config=configs[selectedConfig^]);
    ();
  };

  let goToMainPage = () => {
    getMainPage();
    ReasonReactRouter.push("/" ++ configs[selectedConfig^].network);
  };

  let goToNetwork = network => {
    let selectedIndex =
      configs |> Js.Array.findIndex(conf => conf.network === network);
    switch (selectedIndex) {
    | (-1) => goToMainPage()
    | _ =>
      dispatch(ChangeNetwork(selectedIndex));
      selectedConfig := selectedIndex;
      getMainPage();
    };
  };

  let goToPage = (network: string, entity: string, id: string) => {
    let selectedIndex =
      configs |> Js.Array.findIndex(conf => conf.network === network);
    let isNumber = id |> Utils.isNumber;
    switch (selectedIndex) {
    | (-1) => goToMainPage()
    | _ =>
      dispatch(ChangeNetwork(selectedIndex));
      selectedConfig := selectedIndex;
      switch (entity, isNumber) {
      | ("blocks", false) => getBlock(id, false, false, 0)
      | ("blocks", true) => getHashByLevel(id |> int_of_string, false)
      | ("accounts", false) => getAccount(id, false)
      | ("operations", false) => getOperation(id, false)
      | _ => goToMainPage()
      };
    };
  };

  let setFooterRef = ref => {
    footerRef := Js.Nullable.toOption(ref);
  };

  let setFocusOfSearch = () => {
    switch (footerRef^) {
    | Some(el) =>
      let _ =
        Js.Global.setTimeout(
          _ => {
            let elementObj = ReactDOMRe.domElementToObj(el);
            ignore(elementObj##focus());
            elementObj##select();
          },
          100,
        );
      ();
    | None => ignore()
    };
  };
  let onChangeId = id => {
    let newId = id |> Js.String.replaceByRe([%re "/ /g"], "");
    dispatch(SetId(newId));
  };

  let onSearchById = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let twoChars =
      id |> Js.String.slice(~from=0, ~to_=2) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, twoChars, isNumber) {
    | ("b", _, _) => getBlock(id, true, false, 0)
    | ("o", _, _) => getOperation(id, true)
    | (_, "tz", _)
    | (_, "kt", _) => getAccount(id, true)
    | (_, _, true) => getHashByLevel(id |> int_of_string, false)
    | _ => dispatch(SetError(Utils.invalidId))
    };
  };

  let onSearchMain = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, isNumber) {
    | ("b", _) => getBlock(id, true, true, 0)
    | (_, true) => getHashByLevel(id |> int_of_string, true)
    | _ => dispatch(SetError(Utils.invalidId))
    };
  };

  let onChangeNetwork = (index: int) =>
    if (state.selectedConfig !== index) {
      dispatch(ChangeNetwork(index));
      selectedConfig := index;
      goToMainPage();
    } else {
      dispatch(OpenNetwork(false));
    };

  React.useEffect0(() => {
    switch (url.path) {
    | [network, entity, id] => goToPage(network, entity, id)
    | [network] => goToNetwork(network)
    | _ => goToMainPage()
    };
    None;
  });

  <ReactIntl.IntlProvider>
    <ContextProvider value={state.selectedConfig}>
      <div
        className={Styles.container(state.selectedConfig)}
        tabIndex=0
        onKeyPress={event =>
          switch (event |> ReactEvent.Keyboard.key) {
          | "s"
          | "S" => setFocusOfSearch()
          | _ => ignore()
          }
        }>
        <div className=Styles.container1>
          <div className=Styles.header>
            <div className=Styles.headerTitle onClick={_ => goToMainPage()}>
              {ReasonReact.string("MININAX")}
            </div>
          </div>
          <div className=Styles.mainContainer>
            {switch (url.path) {
             | [_, "accounts", _] =>
               <Account items={state.account} goToDetail=onSearchById />
             | [_, "operations", _] =>
               <Operation items={state.operation} goToDetail=onSearchById />
             | [_, "blocks", _] =>
               <Block
                 items={state.block}
                 goToDetail=onSearchById
                 changeLevel={level => getHashByLevel(level, false)}
               />
             | _ =>
               <Dashboard
                 items={state.lastBlock}
                 blockinfo={state.blockinfo}
                 transinfo={state.transinfo}
                 voteinfo={state.voteinfo}
                 proposals={state.proposals}
                 onSearch={val_ => onSearchMain(val_)}
                 changeLevel={level => getHashByLevel(level, true)}
               />
             }}
          </div>
          <Footer
            searchVal={state.id}
            network={configs[state.selectedConfig].displayName}
            setRef=setFooterRef
            changeId=onChangeId
            onSearch={_ => onSearchById(state.id)}
            onOpenNetworkSelector={_ => dispatch(OpenNetwork(true))}
          />
          {state.isLoading ? <Loader /> : ReasonReact.null}
          {state.isError
             ? <Error
                 error={state.error}
                 onTry={_ => dispatch(RemoveError)}
               />
             : ReasonReact.null}
          {state.isOpenNetworkSelector
             ? <NetworkSelector
                 selectedIndex={state.selectedConfig}
                 onChange=onChangeNetwork
                 onCancel={_ => dispatch(OpenNetwork(false))}
               />
             : ReasonReact.null}
        </div>
      </div>
    </ContextProvider>
  </ReactIntl.IntlProvider>;
};