open GlobalStore;
open Configs;

let selector = state => state;

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

let selectedConfig = ref(0);

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let state = AppStore.useSelector(selector);
  let (footerRef, setFooterRef) = React.useState(() => None);
  let changeFooterRef = ref =>
    setFooterRef(_old => Js.Nullable.toOption(ref));
  module UseApp =
    AppHook.Make({});
  open UseApp;

  let onKeyPress =
    React.useCallback0(event =>
      switch (event |> ReactEvent.Keyboard.key) {
      | "s"
      | "S" => setFocusOfSearch(footerRef)
      | _ => ignore()
      }
    );

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
        onKeyPress>
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
            setRef=changeFooterRef
            changeId=onChangeId
            onSearch={_ => onSearchById(state.id)}
            onOpenNetworkSelector={_ => dispatch(OpenNetwork(true))}
          />
          <If validator={state.isLoading}> <Loader /> </If>
          <If validator={state.isError}>
            <Error error={state.error} onTry={_ => dispatch(RemoveError)} />
          </If>
          <If validator={state.isOpenNetworkSelector}>
            <NetworkSelector
              selectedIndex={state.selectedConfig}
              onChange=onChangeNetwork
              onCancel={_ => dispatch(OpenNetwork(false))}
            />
          </If>
        </div>
      </div>
    </ContextProvider>
  </ReactIntl.IntlProvider>;
};