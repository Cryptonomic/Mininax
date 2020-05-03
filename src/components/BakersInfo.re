open ReactIntl;
open GlobalStore;

let str = ReasonReact.string;
let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());
let selector = (state: GlobalStore.globalState) =>
  state.dashboardState.bakersInfo;

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let bakersInfo = Store.useSelector(selector);

  let tez_staked =
    switch (bakersInfo.bakersSumStakingBalance) {
    | Some(value) =>
      intl->Intl.formatNumberWithOptions(
        Utils.convertFromUtezfToTez(value),
        numFormatOptions,
      )
      |> Helpers.toOption
    | None => None
    };

  let total_tez =
    switch (bakersInfo.totalTez) {
    | Some(value) =>
      intl->Intl.formatNumberWithOptions(
        Utils.convertFromUtezfToTez(value),
        numFormatOptions,
      )
      |> Helpers.toOption
    | None => None
    };

  let percent_staked =
    switch (bakersInfo.bakersSumStakingBalance, bakersInfo.totalTez) {
    | (Some(bakersSumStakingBalance), Some(totalTez)) =>
      Js.Math.round(bakersSumStakingBalance /. totalTez *. 100.0)
      |> Js.Float.toString
      |> Helpers.toOption
    | _ => None
    };

  <div className={DashboardStyles.rightBottomContainer(theme)}>
    /*{ReasonReact.string("There are ")}
      <div className={DashboardStyles.networkContent(theme)}>
        {intl
         ->Intl.formatNumber(float_of_string(blockinfo.num_bakers))
         ->React.string}
      </div>
      {ReasonReact.string(" active bakers. A total of ")}*/

      <IfOption validator=tez_staked>
        <p>
          {"A total of " |> str}
          <span className={DashboardStyles.networkContent(theme)}>
            {Helpers.optionToString(tez_staked) ++ " XTZ" |> str}
          </span>
          {" out of " |> str}
          <span className={DashboardStyles.networkContent(theme)}>
            {Helpers.optionToString(total_tez) ++ " XTZ" |> str}
          </span>
          {" or " |> str}
          <span className={DashboardStyles.networkContent(theme)}>
            {"(" ++ Helpers.optionToString(percent_staked) ++ "%)" |> str}
          </span>
          {" of TEZ, is being staked right now." |> str}
          {"." |> str}
        </p>
      </IfOption>
    </div>;
};