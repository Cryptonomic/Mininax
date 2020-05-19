open GlobalStore;
open ReactIntl;
open DashboardStyles;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.lastBlock,
  state.dashboardState.governanceProcessInfo,
  state.dashboardState.proposalsInfo,
  state.dashboardState.latestGovernance,
);

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let (block, gp, proposals, latestGovernance) = Store.useSelector(selector);

  let percentYay =
    switch (gp.yayRolls, gp.nayRolls) {
    | (Some(yay_rolls), Some(nay_rolls)) =>
      Js.Math.round(
        float_of_int(yay_rolls)
        /. float_of_int(yay_rolls + nay_rolls)
        *. 100.0,
      )
      |> Helpers.toOption
    | _ => None
    };

  let governanceCycle =
    switch (latestGovernance) {
    | Some(value) => value.cycle * 16 / 400 |> Helpers.toOption
    | None => None
    };

  let (rightTitle, proposalsTitle) =
    switch (block.period_kind, proposals) {
    | (Some("proposal"), Some(arr)) when arr |> Array.length == 0 => (
        "PROPOSAL",
        "There are no active proposals.",
      )
    | (Some("proposal"), Some(arr)) when arr |> Array.length == 1 => (
        "PROPOSAL",
        "The active proposal is:",
      )
    | (Some("proposal"), _) => ("PROPOSAL", "The active proposals are:")
    | (Some("testing"), _) => ("TESTING", "The active proposal is:")
    | (Some("testing_vote"), _) => (
        "TESTING VOTE",
        "The active proposal is:",
      )
    | (Some("promotion_vote"), _) => (
        "PROMOTION VOTE",
        "The active proposal is:",
      )
    | _ => ("", "")
    };

  <>
    <div className={DashboardStyles.rightTopContainer(theme)}>
      <p className=inline>
        {"We are currently in the " |> str}
        <span className={DashboardStyles.networkContent(theme)}>
          {rightTitle |> str}
        </span>
      </p>
      <IfElseOption validator=governanceCycle>
        (
          <>
            {" phase of the governance process, that is in " |> str}
            <span className={content1(theme)}>
              {intl->Intl.formatNumber(
                 float_of_int(Helpers.optionToInt(governanceCycle)),
               )
               ++ "/16"
               |> str}
            </span>
            {" cycle." |> str}
          </>,
          <> {" phase of the governance process." |> str} </>,
        )
      </IfElseOption>
    </div>
    <div className={DashboardStyles.rightMdContainer(theme)}>
      <p> {proposalsTitle |> str} </p>
      {switch (block.period_kind) {
       | Some("proposal") =>
         <IfOption validator=proposals>
           <div className=DashboardStyles.rightMdMainContainer>
             {proposals
              |> Helpers.optionToArray
              |> Array.mapi((index, pr: DashboardStore.Types_.proposalInfo) => {
                   let lastTxt =
                     if (index
                         == Js.Array.length(Helpers.optionToArray(proposals))
                         - 1) {
                       " votes.";
                     } else {
                       " votes and ";
                     };
                   <div key={string_of_int(index)}>
                     <IfOption validator={pr.proposal}>
                       <TextWithCopy
                         className={DashboardStyles.content2(theme)}
                         value={pr.proposal |> Helpers.optionToString}
                       />
                       <p className=DashboardStyles.inline>
                         {"with " |> str}
                       </p>
                     </IfOption>
                     <p>
                       <span className={DashboardStyles.content3(theme)}>
                         {intl->Intl.formatNumber(
                            float_of_int(pr.count_operation_group_hash),
                          )
                          |> str}
                       </span>
                       {lastTxt |> str}
                     </p>
                   </div>;
                 })
              |> ReasonReact.array}
           </div>
         </IfOption>
       | Some("testing") =>
         <IfOption validator={block.active_proposal}>
           <div className=DashboardStyles.rightMdMainContainer>
             <TextWithCopy
               className={DashboardStyles.content2(theme)}
               value={block.active_proposal |> Helpers.optionToString}
             />
           </div>
         </IfOption>
       | Some("testing_vote")
       | Some("promotion_vote") =>
         <div className=DashboardStyles.rightMdMainContainer>
           <p>
             <IfOption validator={block.active_proposal}>
               <TextWithCopy
                 className={DashboardStyles.content2(theme)}
                 value={block.active_proposal |> Helpers.optionToString}
               />
             </IfOption>
             <IfOption validator={gp.yayRolls}>
               <span className={DashboardStyles.content3(theme)}>
                 {intl
                  ->Intl.formatNumber(
                      float_of_int(Helpers.optionToInt(gp.yayRolls)),
                    )
                  ->str}
               </span>
               {" rolls have been cast for, " |> str}
             </IfOption>
             <IfOption validator={gp.nayRolls}>
               <span className={DashboardStyles.content3(theme)}>
                 {intl
                  ->Intl.formatNumber(
                      float_of_int(Helpers.optionToInt(gp.nayRolls)),
                    )
                  ->str}
               </span>
               {" against" |> str}
             </IfOption>
             <IfOption validator={gp.passRolls}>
               {" and " |> str}
               <span className={DashboardStyles.content3(theme)}>
                 {intl
                  ->Intl.formatNumber(
                      float_of_int(Helpers.optionToInt(gp.passRolls)),
                    )
                  ->str}
               </span>
               {" have passed." |> str}
             </IfOption>
             <IfOption validator=percentYay>
               <span className={DashboardStyles.content3(theme)}>
                 {Js.Float.toString(Helpers.optionToFloat(percentYay))
                  ++ "%"
                  |> str}
               </span>
               {" of commited rolls were in favor of the proposal, " |> str}
               <If validator={Helpers.optionToFloat(percentYay) < 90.0}>
                 {"not " |> str}
               </If>
               {"exceeding the supermajority requirement of 80%." |> str}
             </IfOption>
           </p>
         </div>
       | _ => ReasonReact.null
       }}
    </div>
  </>;
};