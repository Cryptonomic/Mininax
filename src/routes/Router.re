open GlobalStore;

module Style = {
  open Css;
  let mainContainer = style([flex(`num(1.)), marginBottom(px(57))]);
};
let selector = state => state;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let state = AppStore.useSelector(selector);
  module UseApp =
    AppHook.Make({});
  UseApp.(
    <div className=Style.mainContainer>
      {switch (url.path) {
       | [_, "accounts", _] =>
         <Account items={state.account} goToDetail=onSearchById />
       | [_, "operations", _] when state.operation |> Array.length > 0 =>
         <Operation items={state.operation} goToDetail=onSearchById />
       | [_, "blocks", _] =>
         <Block
           items={state.block}
           goToDetail=onSearchById
           changeLevel={getHashByLevel(~isMain=false)}
         />
       | _ =>
         <Dashboard
           items={state.lastBlock}
           blockinfo={state.blockinfo}
           transinfo={state.transinfo}
           voteinfo={state.voteinfo}
           proposals={state.proposals}
           onSearch=onSearchMain
           changeLevel={getHashByLevel(~isMain=true)}
         />
       }}
    </div>
  );
};