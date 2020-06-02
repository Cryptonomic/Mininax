open ReactIntl;
open GlobalStore;
open DashboardStyles;

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
      intl
      ->Intl.formatNumberWithOptions(
          Utils.convertFromUtezfToTez(value),
          numFormatOptions,
        )
      |> Helpers.toOption
    | None => None
    };

  let total_tez =
    switch (bakersInfo.totalTez) {
    | Some(value) =>
      intl
      ->Intl.formatNumberWithOptions(
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

  let staked =
    switch (tez_staked, total_tez, percent_staked) {
    | (Some(tez), Some(total), Some(percent)) =>
      Some((tez, total, percent))
    | _ => None
    };

  <div className={rightBottomContainer(theme)}>
    /*{ReasonReact.string("There are ")}
      <div className={DashboardStyles.networkContent(theme)}>
        {intl
         ->Intl.formatNumber(float_of_string(blockinfo.num_bakers))
         ->React.string}
      </div>
      {ReasonReact.string(" active bakers. A total of ")}*/

      <IfOption validator=staked>
        {
          ((tez, total, percent)) =>
            <p className=inline>
              {"A total of " |> str}
              <span className={networkContent(theme)}>
                {tez ++ " XTZ" |> str}
              </span>
              {" out of " |> str}
              <span className={networkContent(theme)}>
                {total ++ " XTZ" |> str}
              </span>
              {" or " |> str}
              <span className={networkContent(theme)}>
                {"(" ++ percent ++ "%)" |> str}
              </span>
              {" of TEZ, is being staked right now." |> str}
            </p>
        }
      </IfOption>
      <IfOption validator={bakersInfo.top3Bakers}>
        {
          value =>
            <>
              <p className=inline>
                {" Top 3 bakers at the moment are:" |> str}
              </p>
              {
                value
                |> Array.map((baker: DashboardStore.Types_.baker) =>
                     <div key={baker.countAccountId}>
                       <TextWithCopy
                         className={content1(theme)}
                         value={baker.delegateValue}
                         isReverse=true
                       />
                     </div>
                   )
                |> ReasonReact.array
              }
            </>
        }
      </IfOption>
    </div>;
};