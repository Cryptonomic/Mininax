[@bs.scope "document"] [@bs.val]
external activeElement: Dom.element = "activeElement";
open GlobalStore;
open Configs;

let selector = state => state.appState;

module Style = {
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
      display(flexBox),
      alignItems(`flexEnd),
    ]);
  let headerTitle =
    style([
      fontFamily(`custom("'Arcade', sans-serif")),
      fontSize(px(60)),
      color(white),
      lineHeight(px(30)),
      letterSpacing(px(3)),
      cursor(`pointer),
      display(inlineBlock),
    ]);

  let subTitle =
    merge([headerTitle, style([marginLeft(px(20)), fontSize(px(40))])]);
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let state = GlobalStore.Store.useSelector(selector);
  let (footerRef, setFooterRef) =
    React.useState(() => (None: option(Dom.element)));
  let changeFooterRef = ref => {
    setFooterRef(_old => Js.Nullable.toOption(ref));
  };
  module UseApp =
    AppHook.Make({});
  open UseApp;

  let onKeyPress =
    React.useCallback1(
      event => {
        switch (event |> ReactEvent.Keyboard.key, footerRef) {
        | ("s", Some(x))
        | ("S", Some(x)) when x !== activeElement =>
          setFocusOfSearch(footerRef)
        | _ => ignore()
        }
      },
      [|footerRef|],
    );

  React.useEffect0(() => {
    switch (url.path) {
    | [network, _, _]
    | [network] =>
      let selectedIndex =
        configs
        |> Js.Array.findIndex((conf: MainType.config) =>
             conf.network === network
           );
      dispatch(AppAction(Init(selectedIndex)));
    | _ => dispatch(AppAction(Init(0)))
    };
    let interval = Js.Global.setInterval(() => {getMainPage()}, 600000);
    Some(() => {Js.Global.clearInterval(interval)});
  });

  React.useEffect1(
    () => {
      switch (state.inited) {
      | 0 => ()
      | _ =>
        switch (url.path) {
        | [network, entity, id] => goToPage(network, entity, id)
        | [network] => goToNetwork(network)
        | _ =>
          goToMainPage(configs[state.selectedConfig].network);
        }
      };
      None;
    },
    [|state.inited, state.selectedConfig|],
  );

  <ReactIntl.IntlProvider key={lang || 'en'}  locale={lang || 'en'}>
    <ContextProvider value={state.selectedConfig}>
      <div
        className={Style.container(state.selectedConfig)}
        tabIndex=0
        onKeyPress>
        <div className=Style.container1>
          <div className=Style.header>
            <div
              className=Style.headerTitle
              onClick={_ =>
                goToMainPage(configs[state.selectedConfig].network)
              }>
              {ReasonReact.string("MININAX")}
            </div>
            <div
              className=Style.subTitle
              onClick={_ => Utils.open_("https://arronax.io", "_blank")}>
              {ReasonReact.string("ARRONAX")}
            </div>
            <div
              className=Style.subTitle
              onClick={_ =>
                Utils.open_("http://cryptonomic.tech/galleon.html", "_blank")
              }>
              {ReasonReact.string("GALLEON")}
            </div>
          </div>
          <Router />
          <Footer
            searchVal={state.id}
            network={configs[state.selectedConfig].displayName}
            setRef=changeFooterRef
            changeId=onChangeId
            onSearch={_ => onSearchById(state.id)}
            onOpenNetworkSelector={_ =>
              dispatch(AppAction(OpenNetwork(true)))
            }
          />
          <If validator={state.isLoading}> <Loader /> </If>
          <If validator={state.isError}>
            <Error
              error={state.error}
              onTry={_ => dispatch(AppAction(RemoveError))}
            />
          </If>
          <If validator={state.isOpenNetworkSelector}>
            <NetworkSelector
              selectedIndex={state.selectedConfig}
              onChange=onChangeNetwork
              onCancel={_ => dispatch(AppAction(OpenNetwork(false)))}
            />
          </If>
        </div>
      </div>
    </ContextProvider>
  </ReactIntl.IntlProvider>;
};
