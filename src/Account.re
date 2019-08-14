open Styles1;
open Configs;
[@react.component]
let make = (~items, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let changeLevel = (level) => {
    Js.log2("level", level);
    ();
  };
  let id = Utils.getValueFromDict(items, "account_id");
  let displayName = Utils.getDisplayName(configs[theme]);
  let onOpenAccountSends = (_ev) => {
    let query = Utils.getQueryForAccountSends(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let onOpenAccountReceipts = (_ev) => {
    let query = Utils.getQueryForAccountReceipts(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let onOpenAccountOtherOperations = (_ev) => {
    let query = Utils.getQueryForOtherOperations(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let onOpenAccountEndorsements = (_ev) => {
    let query = Utils.getQueryForEndorsements(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let onOpenAccountBakedBlocks = (_ev) => {
    let query = Utils.getQueryForBakedBlocks(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let onOpenAccountDeposits = (_ev) => {
    let query = Utils.getQueryForDepositsAndRewards(id);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  <div className=container(theme)>
    <div className=title(theme)>{ReasonReact.string("Account")}</div>
    <ItemDisplay entity="account" items changeLevel goToDetail />
    <div className=accountBtn(theme) onClick={onOpenAccountSends}>{ReasonReact.string("All sent transactions")}</div>
    <div className=accountBtn(theme) onClick={onOpenAccountReceipts}>{ReasonReact.string("All received transactions")}</div>
    <div className=accountBtn(theme) onClick={onOpenAccountOtherOperations}>{ReasonReact.string("All reveals, delegations and originations")}</div>
    <div className=accountBtn(theme) onClick={onOpenAccountEndorsements}>{ReasonReact.string("All endorsements")}</div>
    <div className=accountBtn(theme) onClick={onOpenAccountBakedBlocks}>{ReasonReact.string("All baked blocks")}</div>
    <div className=accountBtn(theme) onClick={onOpenAccountDeposits}>{ReasonReact.string("All deposits and rewards")}</div>
  </div>
}
