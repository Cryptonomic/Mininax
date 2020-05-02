open GlobalStore;
open ReactIntl;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.lastBlock,
  state.dashboardState.voteinfo,
  state.dashboardState.proposalsInfo,
);

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let (items, voteinfo, proposals) = Store.useSelector(selector);

  let sumYayNay = float_of_int(voteinfo.yay_rolls + voteinfo.nay_rolls);
  let percentYay =
    Js.Math.round(float_of_int(voteinfo.yay_rolls) /. sumYayNay *. 100.0);
  let periodKind = items##period_kind;

  let (rightTitle, proposalsTitle) =
    switch (periodKind, proposals |> Array.length) {
    | ("proposal", 0) => ("PROPOSAL", "There are no active proposals.")
    | ("proposal", 1) => ("PROPOSAL", "The active proposal is:")
    | ("proposal", _) => ("PROPOSAL", "The active proposals are:")
    | ("testing", _) => ("TESTING", "The active proposal is:")
    | ("testing_vote", _) => ("TESTING VOTE", "The active proposal is:")
    | ("promotion_vote", _) => ("PROMOTION VOTE", "The active proposal is:")
    | _ => ("", "")
    };

  <>
    <div className={DashboardStyles.rightTopContainer(theme)}>
      <p>
        {"We are currently in the " |> str}
        <span className={DashboardStyles.networkContent(theme)}>
          {rightTitle |> str}
        </span>
        {" phase of the governance process." |> str}
      </p>
    </div>
    <div className={DashboardStyles.rightMdContainer(theme)}>
      <p> {proposalsTitle |> str} </p>
      {switch (periodKind) {
       | "proposal" =>
         <div className=DashboardStyles.rightMdMainContainer>
           {proposals
            |> Array.mapi((index, pr: DashboardStore.proposalInfo) => {
                 let lastTxt =
                   if (index == Js.Array.length(proposals) - 1) {
                     " votes.";
                   } else {
                     " votes and ";
                   };
                 <div key={string_of_int(index)}>
                   <TextWithCopy
                     className={DashboardStyles.content2(theme)}
                     value={pr.proposal}
                   />
                   <p>
                     {"with " |> str}
                     <span className={DashboardStyles.content3(theme)}>
                       {intl->Intl.formatNumber(
                          float_of_string(pr.count_operation_group_hash),
                        )
                        |> str}
                     </span>
                     {lastTxt |> str}
                   </p>
                 </div>;
               })
            |> ReasonReact.array}
         </div>
       | "testing" =>
         <div className=DashboardStyles.rightMdMainContainer>
           <TextWithCopy
             className={DashboardStyles.content2(theme)}
             value=items##active_proposal
           />
         </div>
       | "testing_vote"
       | "promotion_vote" =>
         <div className=DashboardStyles.rightMdMainContainer>
           <p>
             <TextWithCopy
               className={DashboardStyles.content2(theme)}
               value=items##active_proposal
             />
             <span className={DashboardStyles.content3(theme)}>
               {intl
                ->Intl.formatNumber(float_of_int(voteinfo.yay_rolls))
                ->str}
             </span>
             {" rolls have been cast for, " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl
                ->Intl.formatNumber(float_of_int(voteinfo.nay_rolls))
                ->str}
             </span>
             {" against and " |> str}
             <span className={DashboardStyles.content3(theme)}>
               {intl
                ->Intl.formatNumber(float_of_int(voteinfo.pass_rolls))
                ->str}
             </span>
             {" have passed." |> str}
             <span className={DashboardStyles.content3(theme)}>
               {Js.Float.toString(percentYay) ++ "%" |> str}
             </span>
             {" of commited rolls were in favor of the proposal, " |> str}
             <If validator={percentYay < 90.0}> {"not " |> str} </If>
             {"exceeding the supermajority requirement of 80%." |> str}
           </p>
         </div>
       | _ => ReasonReact.null
       }}
    </div>
  </>;
};