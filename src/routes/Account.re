open Styles1;
open Configs;
[@react.component]
let make = (~items, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let changeLevel = level => {
    Js.log2("level", level);
    ();
  };
  let id = Utils.getValueFromDict(items, "account_id");
  let displayName = Utils.getDisplayName(configs[theme]);

  let onOpenAccountSends = _ev =>
    Queries.getQueryForAccountSends(id)
    ->Utils.openSharedUrl(displayName, "operations");

  let onOpenAccountReceipts = _ev =>
    Queries.getQueryForAccountReceipts(id)
    ->Utils.openSharedUrl(displayName, "operations");

  let onOpenAccountOtherOperations = _ev =>
    Queries.getQueryForOtherOperations(id)
    ->Utils.openSharedUrl(displayName, "operations");

  let onOpenAccountEndorsements = _ev =>
    Queries.getQueryForEndorsements(id)
    ->Utils.openSharedUrl(displayName, "operations");

  let onOpenAccountBakedBlocks = _ev =>
    Queries.getQueryForBakedBlocks(id)
    ->Utils.openSharedUrl(displayName, "blocks");

  let onOpenAccountDeposits = _ev =>
    Queries.getQueryForDepositsAndRewards(id)
    ->Utils.openSharedUrl(displayName, "balance_updates");

  <div className={container(theme)}>
    <div className={title(theme)}> {ReasonReact.string("Account")} </div>
    <ItemDisplay entity="account" items changeLevel goToDetail />
    <div className={accountBtn(theme)} onClick=onOpenAccountSends>
      {ReasonReact.string("All sent transactions")}
    </div>
    <div className={accountBtn(theme)} onClick=onOpenAccountReceipts>
      {ReasonReact.string("All received transactions")}
    </div>
    <div className={accountBtn(theme)} onClick=onOpenAccountOtherOperations>
      {ReasonReact.string("All reveals, delegations and originations")}
    </div>
    <div className={accountBtn(theme)} onClick=onOpenAccountEndorsements>
      {ReasonReact.string("All endorsements")}
    </div>
    <div className={accountBtn(theme)} onClick=onOpenAccountBakedBlocks>
      {ReasonReact.string("All baked blocks")}
    </div>
    <div className={accountBtn(theme)} onClick=onOpenAccountDeposits>
      {ReasonReact.string("All deposits and rewards")}
    </div>
  </div>;
};