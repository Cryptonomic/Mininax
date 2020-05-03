open Configs;
open GlobalStore;
open ReactIntl;
[@bs.module] external searchSvg: string = "../assets/images/search.svg";

let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());

let selector = (state: GlobalStore.globalState) =>
  state.dashboardState.lastBlock;

[@react.component]
let make = (~onSearch, ~changeLevel) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let (searchVal, setSearchVal) = React.useState(() => "");
  let network = configs[theme].network;
  let items = Store.useSelector(selector);

  let gotoLastBlock = () => {
    let url = Utils.makeUrl(~network, ~entity="blocks", ~id=items##hash);
    ReasonReactRouter.push(url);
  };
  let gotoLastOperations = () => {
    let query = Queries.getQueryForBlockLink(items##hash);
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "operations");
  };
  let gotoBlocksArronax = () => {
    let query = Queries.getQueryForBlocksTab();
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "blocks");
  };
  let onSearchBlock = () =>
    if (searchVal != "") {
      onSearch(searchVal);
    };

  <div className=DashboardStyles.container>
    <div className=DashboardStyles.mainContainer>
      <div className=DashboardStyles.leftContainer>
        <BlocksInfo />
        // <TotalsInfo />
      </div>
      <div className=DashboardStyles.rightContainer>
        // <GovernanceProcessInfo />
        // <BakersInfo />
        <div className=DashboardStyles.rightSearchConainer>
          <div className=DashboardStyles.levelLabelContainer>
            <button
              className={DashboardStyles.levelBtn(theme)}
              onClick={_ => changeLevel(items##level - 1)}>
              {ReasonReact.string("<")}
            </button>
            <div className={DashboardStyles.levelLabel(theme)}>
              {intl
               ->Intl.formatNumber(float_of_int(items##level))
               ->React.string}
            </div>
            <button
              className={DashboardStyles.levelBtn(theme)}
              onClick={_ => changeLevel(items##level + 1)}>
              {ReasonReact.string(">")}
            </button>
          </div>
          <input
            value=searchVal
            onChange={event =>
              setSearchVal(ReactEvent.Form.target(event)##value)
            }
            onKeyDown={event =>
              switch (event |> ReactEvent.Keyboard.which) {
              | 13 => onSearchBlock()
              | _ => ignore()
              }
            }
            className={DashboardStyles.input(theme)}
            placeholder="Show stats for block ID or level"
          />
          <button
            className={DashboardStyles.searhBtn(theme)}
            onClick={_ => onSearchBlock()}>
            <img src=searchSvg />
          </button>
        </div>
      </div>
    </div>
    <div className=DashboardStyles.footContainer>
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoLastBlock()}>
        {ReasonReact.string("Block Head")}
      </div>
      <div className={DashboardStyles.divider(theme)} />
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoBlocksArronax()}>
        {ReasonReact.string("Latest Blocks")}
      </div>
      <div className={DashboardStyles.divider(theme)} />
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoLastOperations()}>
        {ReasonReact.string("Latest Operations")}
      </div>
    </div>
  </div>;
};