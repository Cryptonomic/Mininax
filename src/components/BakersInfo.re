open ReactIntl;
open GlobalStore;

let str = ReasonReact.string;
let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());
let selector = (state: GlobalStore.globalState) =>
  state.dashboardState.blockinfo;

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let blockinfo = Store.useSelector(selector);
  let tez_staked =
    intl->Intl.formatNumberWithOptions(
      Utils.convertFromUtezfToTez(blockinfo.bakers_sum_staking_balance),
      numFormatOptions,
    );
  let total_tez =
    intl->Intl.formatNumberWithOptions(
      Utils.convertFromUtezfToTez(blockinfo.totalTez),
      numFormatOptions,
    );
  let percent_staked =
    Js.Math.round(
      blockinfo.bakers_sum_staking_balance /. blockinfo.totalTez *. 100.0,
    );

  <div className={DashboardStyles.rightBottomContainer(theme)}>
    /*{ReasonReact.string("There are ")}
      <div className={DashboardStyles.networkContent(theme)}>
        {intl
         ->Intl.formatNumber(float_of_string(blockinfo.num_bakers))
         ->React.string}
      </div>
      {ReasonReact.string(" active bakers. A total of ")}*/

      <p>
        {"A total of " |> str}
        <span className={DashboardStyles.networkContent(theme)}>
          {tez_staked ++ " XTZ" |> str}
        </span>
        {" out of " |> str}
        <span className={DashboardStyles.networkContent(theme)}>
          {total_tez ++ " XTZ" |> str}
        </span>
        {" or " |> str}
        <span className={DashboardStyles.networkContent(theme)}>
          {ReasonReact.string(
             "(" ++ Js.Float.toString(percent_staked) ++ "%)",
           )}
        </span>
        {" of TEZ, is being staked right now." |> str}
      </p>
    </div>;
};