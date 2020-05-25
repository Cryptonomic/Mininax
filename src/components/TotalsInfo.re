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
      intl->Intl.formatNumber(Utils.convertFromUtezToTez(value))
      // compactFormat,
      |> toOption
    | None => None
    };

  let reveals =
    switch (info.reveals, info.contractDeployed) {
    | (Some(reveals), Some(contractDeployed)) =>
      Some((reveals, contractDeployed))
    | _ => None
    };

  let fundraiserCount =
    switch (info.totalFundraiserCount, fundraiserPercent) {
    | (Some(totalFundraiserCount), Some(fundraiserPercent)) =>
      Some((totalFundraiserCount, fundraiserPercent))
    | _ => None
    };

  <div className={DashboardStyles.leftBottomContainer(theme)}>
    <p>
      {"In the past day there have been " |> str}
      <IfOption validator={info.countTransactions}>
        {value =>
           <>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {" transactions " |> str}
           </>}
      </IfOption>
      <IfOption validator={info.countAmount}>
        {value =>
           <>
             {" and " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value))->str}
             </span>
             {" contract invocations" |> str}
           </>}
      </IfOption>
      <IfOption validator=transactions_total_xtz>
        {value =>
           <>
             {" for a total of " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {value ++ " XTZ" |> str}
             </span>
             {" while " |> str}
           </>}
      </IfOption>
      <IfOption validator={info.countOriginatedContracts}>
        {countOriginatedContracts =>
           <If validator={countOriginatedContracts > 0}>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(
                  float_of_int(optionToInt(info.countOriginatedContracts)),
                )
                |> str}
             </span>
             {" accounts were originated, " |> str}
           </If>}
      </IfOption>
      <IfOption validator={info.fundraiserCount}>
        {value =>
           <>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             <IfElse validator={network == "mainnet"}>
               (" fundraiser " |> str, " faucet " |> str)
             </IfElse>
             {"accounts were activated, " |> str}
           </>}
      </IfOption>
      <IfOption validator={info.reveals}>
        {value =>
           <>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {" were revealed" |> str}
           </>}
      </IfOption>
      <IfOption validator=reveals>
        {((_, contractDeployed)) =>
           <>
             {" and " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(contractDeployed))
                |> str}
             </span>
             <IfElse validator={contractDeployed > 1}>
               (" contracts were" |> str, " contract was" |> str)
             </IfElse>
             {" deployed." |> str}
           </>}
      </IfOption>
      <IfOption validator=sumFee>
        {value =>
           <>
             {" A total of " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {value ++ " XTZ" |> str}
             </span>
             {" in fees have been paid out and " |> str}
           </>}
      </IfOption>
      <IfOption validator=consumedGas>
        {value =>
           <>
             <span className={DashboardStyles.content3(theme)}>
               {value |> str}
             </span>
             {" gas has been consumed." |> str}
           </>}
      </IfOption>
      <IfOption validator=fundraiserCount>
        {((totalFundraiserCount, fundraiserPercent)) =>
           <>
             {" There have been " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(totalFundraiserCount))
                ++ " of 30,317"
                |> str}
             </span>
             <IfElse validator={network == "mainnet"}>
               (" fundraiser " |> str, " faucet " |> str)
             </IfElse>
             {"accounts " |> str}
             <span className={DashboardStyles.content2(theme)}>
               {"(" ++ Js.Float.toString(fundraiserPercent) ++ "%)" |> str}
             </span>
             {"  activated so far." |> str}
           </>}
      </IfOption>
      <IfOption validator={info.storageDelta}>
        {value =>
           <>
             {" The last 24 h storage delta is " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {"." |> str}
           </>}
      </IfOption>
    </p>
  </div>;
};