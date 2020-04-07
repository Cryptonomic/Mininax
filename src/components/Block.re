open Configs;
open Helpers;
[@react.component]
let make = (~items, ~changeLevel, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let onOpenUrl = _ev =>
    Utils.openSharedUrl
    ->apply1(
        Utils.getValueFromDict(items, "hash") |> Queries.getQueryForBlockLink,
      )
    ->apply1(Utils.getDisplayName(configs[theme]))
    ->apply1("operations");

  <div className={Styles1.container(theme)}>
    <div className={Styles1.title(theme)}>
      {ReasonReact.string("Block")}
    </div>
    <ItemDisplay entity="block" items changeLevel goToDetail />
    <div className={Styles1.linkBtn(theme)} onClick=onOpenUrl>
      {ReasonReact.string("All Operations >>")}
    </div>
  </div>;
};