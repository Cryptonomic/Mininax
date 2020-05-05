open GlobalStore;
open ReactIntl;
open Helpers;
open Configs;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) =>
  state.dashboardState.totalsInfo;

let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let network = configs[theme].network;
  let intl = ReactIntl.useIntl();
  let info = Store.useSelector(selector);
  let transactions_total_xtz =
    switch (info.sumAmount) {
    | Some(value) =>
      intl->Intl.formatNumberWithOptions(
        Utils.convertFromUtezToTez(value),
        numFormatOptions,
      )
      |> toOption
    | None => None
    };
  let fundraiserPercent =
    switch (info.totalFundraiserCount) {
    | Some(value) =>
      Js.Math.round(float_of_int(value) /. 30317.0 *. 100.0) |> toOption
    | None => None
    };
  let sumFee =
    switch (info.sumFee) {
    | Some(value) =>
      intl->Intl.formatNumberWithOptions(
        Utils.convertFromUtezToTez(value),
        numFormatOptions,
      )
      |> toOption
    | None => None
    };
  let consumedGas =
    switch (info.sumConsumedGas) {
    | Some(value) =>
      intl->Intl.formatNumberWithOptions(
        Utils.convertFromUtezToTez(value),
        numFormatOptions,
      )
      |> toOption
    | None => None
    };

  <div className={DashboardStyles.leftBottomContainer(theme)}>
    <p>
      {"In the past day there have been " |> str}
      <IfOption validator={info.countTransactions}>
        <span className={DashboardStyles.content3(theme)}>
          {intl
           ->Intl.formatNumber(
               float_of_int(optionToInt(info.countTransactions)),
             )
           ->str}
        </span>
        {" transactions " |> str}
      </IfOption>
      <IfOption validator={info.countAmount}>
        {" and " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {intl
           ->Intl.formatNumber(float_of_int(optionToInt(info.countAmount)))
           ->str}
        </span>
        {" contract invocations" |> str}
      </IfOption>
      <IfOption validator=transactions_total_xtz>
        {" for a total of " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {optionToString(transactions_total_xtz) ++ " XTZ" |> str}
        </span>
        {" while " |> str}
      </IfOption>
      <IfOption validator={info.countOriginatedContracts}>
        <If validator={info.countOriginatedContracts |> optionToInt > 0}>
          <span className={DashboardStyles.content3(theme)}>
            {intl
             ->Intl.formatNumber(
                 float_of_int(optionToInt(info.countOriginatedContracts)),
               )
             ->str}
          </span>
          {" accounts were originated, " |> str}
        </If>
      </IfOption>
      <IfOption validator={info.fundraiserCount}>
        <span className={DashboardStyles.content3(theme)}>
          {intl
           ->Intl.formatNumber(
               float_of_int(optionToInt(info.fundraiserCount)),
             )
           ->str}
        </span>
        <IfElse validator={network == "mainnet"}>
          (" fundraiser " |> str, " faucet " |> str)
        </IfElse>
        {"accounts were activated, " |> str}
      </IfOption>
      <IfOption validator={info.reveals}>
        <span className={DashboardStyles.content3(theme)}>
          {intl
           ->Intl.formatNumber(float_of_int(optionToInt(info.reveals)))
           ->str}
        </span>
        {" were revealed" |> str}
      </IfOption>
      <IfOption validator={info.reveals}>
        {" and " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {intl
           ->Intl.formatNumber(
               float_of_int(optionToInt(info.contractDeployed)),
             )
           ->str}
        </span>
        <IfElse validator={optionToInt(info.contractDeployed) > 1}>
          (" contracts were" |> str, " contract was" |> str)
        </IfElse>
        {" deployed." |> str}
      </IfOption>
      <IfOption validator=sumFee>
        {" A total of " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {optionToString(sumFee) ++ " XTZ" |> str}
        </span>
        {" in fees have been paid out and " |> str}
      </IfOption>
      <IfOption validator=consumedGas>
        <span className={DashboardStyles.content3(theme)}>
          {optionToString(consumedGas) |> str}
        </span>
        {" gas has been consumed." |> str}
      </IfOption>
      <IfOption validator={info.totalFundraiserCount}>
        {" There have been " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {intl->Intl.formatNumber(
             float_of_int(optionToInt(info.totalFundraiserCount)),
           )
           ++ " of 30,317"
           |> str}
        </span>
        <IfElse validator={network == "mainnet"}>
          (" fundraiser " |> str, " faucet " |> str)
        </IfElse>
        {"accounts " |> str}
        <span className={DashboardStyles.content2(theme)}>
          {"("
           ++ Js.Float.toString(optionToFloat(fundraiserPercent))
           ++ "%)"
           |> str}
        </span>
        {"  activated so far." |> str}
      </IfOption>
      <IfOption validator={info.storageDelta}>
        {" The last 24 h storage delta is " |> str}
        <span className={DashboardStyles.content3(theme)}>
          {intl->Intl.formatNumber(
             float_of_int(optionToInt(info.storageDelta)),
           )
           |> str}
        </span>
        {"." |> str}
      </IfOption>
    </p>
  </div>;
};