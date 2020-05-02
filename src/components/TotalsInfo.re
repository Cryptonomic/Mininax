open GlobalStore;
open ReactIntl;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.blockinfo,
  state.dashboardState.transinfo,
);

let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let (blockinfo, transinfo) = Store.useSelector(selector);
  let transactions_total_xtz =
    intl->Intl.formatNumberWithOptions(
      Utils.convertFromUtezToTez(transinfo.sumAmount),
      numFormatOptions,
    );
  let fundraiserPercent =
    Js.Math.round(
      float_of_string(blockinfo.totalFundraiserCount) /. 30317.0 *. 100.0,
    );
  let sumFee =
    intl->Intl.formatNumberWithOptions(
      Utils.convertFromUtezToTez(blockinfo.sum_fee),
      numFormatOptions,
    );
  let consumedGas =
    intl->Intl.formatNumberWithOptions(
      Utils.convertFromUtezToTez(blockinfo.sum_consumed_gas),
      numFormatOptions,
    );

  <div className={DashboardStyles.leftBottomContainer(theme)}>
    <p>
      {"In the past day there have been " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {intl->Intl.formatNumber(float_of_string(transinfo.countAmount))->str}
      </span>
      {" transactions for a total of " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {transactions_total_xtz ++ " XTZ" |> str}
      </span>
      {" while " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {intl
         ->Intl.formatNumber(
             float_of_string(transinfo.countOriginatedContracts),
           )
         ->str}
      </span>
      {" accounts were originated and " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {intl
         ->Intl.formatNumber(float_of_string(blockinfo.fundraiserCount))
         ->str}
      </span>
      {" fundraiser accounts were activated. A total of " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {sumFee ++ " XTZ" |> str}
      </span>
      {" in fees have been paid out and " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {consumedGas |> str}
      </span>
      {" gas has been consumed. There have been " |> str}
      <span className={DashboardStyles.content3(theme)}>
        {intl->Intl.formatNumber(
           float_of_string(blockinfo.totalFundraiserCount),
         )
         ++ " of 30,317"
         |> str}
      </span>
      {" fundraiser accounts " |> str}
      <span className={DashboardStyles.content2(theme)}>
        {"(" ++ Js.Float.toString(fundraiserPercent) ++ "%)" |> str}
      </span>
      {"  activated so far." |> str}
    </p>
  </div>;
};