open Configs;
[@react.component]
let make = (~items, ~goToBlock, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let onOpenUrl = () => {
    let hash = switch (Js.Dict.get(items, "hash")) {
      | None => ""
      | Some(value) => value
    };
    let query = Utils.getQueryForBlockLink(hash);
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "operations");

  };
  <div className=Styles1.container(theme)>
    <div className=Styles1.title(theme)>{ReasonReact.string("Block")}</div>
    <ItemDisplay entity="block" items=items changeLevel={goToBlock} goToDetail={goToDetail} />
    <div className=Styles1.linkBtn(theme)>{ReasonReact.string("All Operations >>")}</div>
  </div>
}
