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
  | SetLastBlock(lastBlock, blockinfo, transinfo) =>
    Js.log(blockinfo);
    {...state, lastBlock, transinfo, blockinfo, isLoading: false};
  | SetProposals(proposals) => {...state, proposals}
  | SetVoteInfo(voteinfo) => {...state, voteinfo}
  };

let selectedConfig = ref(0);

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (selectedConfig, setSelectedConfig) = React.useState(() => 0);
  let footerRef = ref(None);
  let (state, dispatch) = React.useReducer(reducer, initState);

  let getProposalsInfo = (metaCycle: int) =>
    ApiCall.getProposalInfoThunk(
      ~metaCycle,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(proposals) => dispatch(SetProposals(proposals))
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getVoteInfo = (hash: string, active_proposal: string) =>
    ApiCall.getVoteInfoThunk(
      ~hash,
      ~active_proposal,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(voteinfo) => dispatch(SetVoteInfo(voteinfo))
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    ApiCall.getBlockInfoThunk(
      ~metaCycle=block##meta_cycle,
      ~timestamp=block##timestamp,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some((blockinfo, transinfo)) =>
          dispatch(SetLastBlock(block, blockinfo, transinfo))
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getBlock = (id: string, isRoute: bool, isMain: bool, level: int) => {
    dispatch(SetLoading);
    let callback = result =>
      switch (result) {
      | Ok((block, lastBlock)) =>
        switch (isRoute, isMain, level) {
        | (true, false, 0) =>
          let url =
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="blocks",
              ~id,
            );
          ReasonReactRouter.push(url);
          dispatch(SetBlock(block, id, false));
        | (true, false, _) =>
          let strLevel = string_of_int(level);
          let url =
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="blocks",
              ~id=strLevel,
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
    ApiCall.getBlockThunk(~callback, ~id, ~config=configs[selectedConfig]);
  };

  let getOperation = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    ApiCall.getOperationThunk(
      ~id,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Ok(operations) when isRoute == true => {
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="operations",
              ~id,
            )
            |> ReasonReactRouter.push;
            dispatch(SetOperations(operations, id));
          }
        | Ok(operations) => dispatch(SetOperations(operations, id))
        | Error(err) => dispatch(SetError(err)),
    );
  };

  let getAccount = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
    ApiCall.getAccountThunk(
      ~id,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Ok(account) when isRoute => {
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="accounts",
              ~id,
            )
            |> ReasonReactRouter.push;
            dispatch(SetAccount(account, id));
          }
        | Ok(account) => dispatch(SetAccount(account, id))
        | Error(err) => dispatch(SetError(err)),
    );
  };

  let getHashByLevel = (level: int, isMain: bool) => {
    dispatch(SetLoading);
    ApiCall.getBlockHashThunk(
      ~level,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(head) when isMain => getBlock(head##hash, false, true, level)
        | Some(head) => getBlock(head##hash, true, false, level)
        | _err => dispatch(SetError(ErrMessage.noAvailable)),
    );
  };

  let getMainPage = () => {
    dispatch(SetLoading);
    let callback = result =>
      switch (result) {
      | Some(head) => getBlock(head##hash, false, true, 0)
      | None => dispatch(SetError(ErrMessage.noAvailable))
      };
    ApiCall.getBlockHeadThunk(~callback, ~config=configs[selectedConfig]);
    ();
  };

  let goToMainPage = () => {
    getMainPage();
    ReasonReactRouter.push("/" ++ configs[selectedConfig].network);
  };

  let goToNetwork = network => {
    let selectedIndex =
      configs |> Js.Array.findIndex(conf => conf.network === network);
    switch (selectedIndex) {
    | (-1) => goToMainPage()
    | _ =>
      dispatch(ChangeNetwork(selectedIndex));
      setSelectedConfig(_old => selectedIndex);
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
      setSelectedConfig(_old => selectedIndex);
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
    | _ => dispatch(SetError(ErrMessage.invalidId))
    };
  };

  let onSearchMain = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, isNumber) {
    | ("b", _) => getBlock(id, true, true, 0)
    | (_, true) => getHashByLevel(id |> int_of_string, true)
    | _ => dispatch(SetError(ErrMessage.invalidId))
    };
  };

  let onChangeNetwork = (index: int) =>
    if (state.selectedConfig !== index) {
      dispatch(ChangeNetwork(index));
      setSelectedConfig(_old => index);
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