open GlobalStore;

module Style = {
  open Css;
  let mainContainer = style([flex(`num(1.)), marginBottom(px(57))]);
};
let selector = state => state.appState;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let state = Store.useSelector(selector);
  module UseApp =
    AppHook.Make({});
  UseApp.(
    <div className=Style.mainContainer>
      {switch (url.path) {
       | [_, "accounts", _] =>
         <Account items={state.account} goToDetail=onSearchById />
       | [_, "operations", _] =>
         <Operation items={state.operation} goToDetail=onSearchById />
       | [_, "blocks", _] =>
         <Block
           items={state.block}
           goToDetail=onSearchById
           changeLevel={getHashByLevel(~isMain=false)}
         />
       | _ =>
         <Dashboard
           onSearch=onSearchMain
           changeLevel={getHashByLevel(~isMain=true)}
         />
       }}
    </div>
  );
};