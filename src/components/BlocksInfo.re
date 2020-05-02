open Configs;
open ReactIntl;
open GlobalStore;
open MomentRe;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.lastBlock,
  state.dashboardState.blockinfo,
);

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let network = configs[theme].network;
  let intl = ReactIntl.useIntl();
  let (items, blockinfo) = Store.useSelector(selector);

  let blocksPerCycle = Utils.getBlocksPerCycle(network);
  let percentBaked =
    Js.Math.round(
      float_of_int(blockinfo.blockCount)
      /. float_of_int(blocksPerCycle)
      *. 100.0,
    );
  let secTimestamp = Utils.getSecondTimeFromMilli(items##timestamp);
  let latestBlockDate =
    momentWithUnix(secTimestamp) |> Moment.format("MMM DD, YYYY");
  let latestBlockTime =
    momentWithUnix(secTimestamp) |> Moment.format("HH:mm");

  <div className={DashboardStyles.leftTopContainer(theme)}>
    <p>
      {"Greetings! The Tezos " |> str}
      <span className={DashboardStyles.networkContent(theme)}>
        {network |> str}
      </span>
      {" is now in cycle " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {intl->Intl.formatNumber(float_of_int(items##meta_cycle))->str}
      </span>
      {"." |> str}
    </p>
    <p>
      {"Within this cycle, " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {intl->Intl.formatNumber(float_of_int(blockinfo.blockCount))
         ++ " of "
         ++ intl->Intl.formatNumber(float_of_int(blocksPerCycle))
         |> str}
      </span>
      {" blocks " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {"(" ++ Js.Float.toString(percentBaked) ++ "%)" |> str}
      </span>
      {" have been baked." |> str}
    </p>
    <p>
      {"The latest block " |> str}
      <TextWithCopy
        className={DashboardStyles.content1(theme)}
        value=items##hash
        isReverse=true
      />
      {" at level " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {intl->Intl.formatNumber(float_of_int(items##level))->str}
      </span>
      {" was baked by " |> str}
      <TextWithCopy
        className={DashboardStyles.content1(theme)}
        value=items##baker
        isReverse=true
      />
      {" at " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {latestBlockTime |> str}
      </span>
      {" on " |> str}
      <span className={DashboardStyles.content1(theme)}>
        {latestBlockDate |> str}
      </span>
      {"." |> str}
    </p>
  </div>;
};