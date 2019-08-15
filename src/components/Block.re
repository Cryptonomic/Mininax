open Configs;
[@react.component]
let make = (~items, ~changeLevel, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let onOpenUrl = (_ev) => {
    let hash = Utils.getValueFromDict(items, "hash");
    let query = Utils.getQueryForBlockLink(hash);
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "operations");
  };
  <div className=Styles1.container(theme)>
    <div className=Styles1.title(theme)>{ReasonReact.string("Block")}</div>
    <ItemDisplay entity="block" items changeLevel goToDetail />
    <div className=Styles1.linkBtn(theme) onClick={onOpenUrl}>{ReasonReact.string("All Operations >>")}</div>
  </div>
}
