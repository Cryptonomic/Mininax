open Styles1;
[@react.component]
let make = (~items, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let changeLevel = (level) => {
    ();
  };
  <div className=container(theme)>
    <div className=title(theme)>{ReasonReact.string("Account")}</div>
    <ItemDisplay entity="account" items=items changeLevel={changeLevel} goToDetail={goToDetail} />
    <div className=accountBtn(theme)>{ReasonReact.string("All sent transactions")}</div>
    <div className=accountBtn(theme)>{ReasonReact.string("All received transactions")}</div>
    <div className=accountBtn(theme)>{ReasonReact.string("All reveals, delegations and originations")}</div>
    <div className=accountBtn(theme)>{ReasonReact.string("All endorsements")}</div>
    <div className=accountBtn(theme)>{ReasonReact.string("All baked blocks")}</div>
    <div className=accountBtn(theme)>{ReasonReact.string("All deposits and rewards")}</div>
  </div>
}
