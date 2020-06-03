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
      value |> float_of_int |> Utils.largeNumberabbreviation |> toOption
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
           <If validator={value > 0}>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {" transactions " |> str}
           </If>}
      </IfOption>
      <IfOption validator={info.countAmount}>
        {value =>
           <If validator={value > 0}>
             {" and " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {" contract invocations" |> str}
           </If>}
      </IfOption>
      <IfOption validator=transactions_total_xtz>
        {value =>
           <If validator={value != "0"}>
             {" for a total of " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {value ++ " XTZ" |> str}
             </span>
             {" while " |> str}
           </If>}
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
             {" contracts were deployed, " |> str}
           </If>}
      </IfOption>
      <IfOption validator={info.fundraiserCount}>
        {value =>
           <If validator={value > 0}>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             <IfElse validator={network == "mainnet"}>
               (" fundraiser " |> str, " faucet " |> str)
             </IfElse>
             {"accounts were activated, " |> str}
           </If>}
      </IfOption>
      <IfOption validator={info.reveals}>
        {value =>
           <If validator={value > 0}>
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(value)) |> str}
             </span>
             {" keys were revealed" |> str}
           </If>}
      </IfOption>
      <IfOption validator=reveals>
        {((_, contractDeployed)) =>
           <If validator={contractDeployed > 0}>
             {" and " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl->Intl.formatNumber(float_of_int(contractDeployed))
                |> str}
             </span>
             <IfElse validator={contractDeployed > 1}>
               (" contracts were" |> str, " contract was" |> str)
             </IfElse>
             {" deployed." |> str}
           </If>}
      </IfOption>
      <IfOption validator=sumFee>
        {value =>
           <If validator={value != "0"}>
             {" A total of " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {value ++ " XTZ" |> str}
             </span>
             {" in fees have been spent and " |> str}
           </If>}
      </IfOption>
      <IfOption validator=consumedGas>
        {value =>
           <If validator={value != "0"}>
             <span className={DashboardStyles.content3(theme)}>
               {value |> str}
             </span>
             {" gas has been consumed." |> str}
           </If>}
      </IfOption>
      <IfOption validator=fundraiserCount>
        {((totalFundraiserCount, fundraiserPercent)) =>
           <If validator={totalFundraiserCount > 0}>
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
           </If>}
      </IfOption>
      <IfOption validator={info.storageDelta}>
        {value =>
           <If validator={value > 0}>
             {" Finally, " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {
                   intl->Intl.formatNumber(float_of_int(value) /. 1000.0)
                   ++ " XTZ"
                   |> str}
             </span>
             {" was spent on storage." |> str}
           </If>}
      </IfOption>
    </p>
  </div>;
};