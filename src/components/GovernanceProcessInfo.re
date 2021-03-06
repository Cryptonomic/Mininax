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

  let cycleAndPeriod =
    switch (block.meta_cycle, block.meta_voting_period) {
    | (Some(meta_cycle), Some(meta_voting_period)) =>
      Some((meta_cycle, meta_voting_period))
    | _ => None
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
          value =>
            <>
              {" phase of the governance process, that is in cycle " |> str}
              <span className={content1(theme)}>
                {
                  intl->Intl.formatNumber(float_of_int(value)) ++ " of 16" |> str
                }
              </span>
              <IfOption validator=cycleAndPeriod>
                {((meta_cycle, meta_voting_period)) =>
                  <>
                    {" of period " |> str}
                    <span className={content1(theme)}>
                      {intl->Intl.formatNumber(float_of_int(meta_voting_period))
                      |> str}
                    </span>
                  </>}
              </IfOption>
              {"." |> str}
            </>,
          <> {" phase of the governance process." |> str} </>,
        )
      </IfElseOption>
    </div>
    <div className={DashboardStyles.rightMdContainer(theme)}>
      <p> {proposalsTitle |> str} </p>
      {
        switch (block.period_kind) {
        | Some("proposal") =>
          <IfOption validator=proposals>
            (
              proposalsValue =>
                <div className=DashboardStyles.rightMdMainContainer>
                  {
                    proposalsValue
                    |> Array.mapi(
                         (index, pr: DashboardStore.Types_.proposalInfo) => {
                         let lastTxt =
                           if (index
                               == Js.Array.length(
                                    Helpers.optionToArray(proposals),
                                  )
                               - 1) {
                             " votes.";
                           } else {
                             " votes and ";
                           };
                         <div key={string_of_int(index)}>
                           <IfOption validator={pr.proposal}>
                             {
                               value =>
                                 <>
                                   <TextWithCopy
                                     className={
                                       DashboardStyles.content2(theme)
                                     }
                                     value
                                   />
                                   <p className=DashboardStyles.inline>
                                     {"with " |> str}
                                   </p>
                                 </>
                             }
                           </IfOption>
                           <p>
                             <span
                               className={DashboardStyles.content3(theme)}>
                               {
                                 intl
                                 ->Intl.formatNumber(
                                     float_of_int(
                                       pr.count_operation_group_hash,
                                     ),
                                   )
                                 |> str
                               }
                             </span>
                             {lastTxt |> str}
                           </p>
                         </div>;
                       })
                    |> ReasonReact.array
                  }
                </div>
            )
          </IfOption>
        | Some("testing") =>
          <IfOption validator={block.active_proposal}>
            (
              value =>
                <div className=DashboardStyles.rightMdMainContainer>
                  <TextWithCopy
                    className={DashboardStyles.content2(theme)}
                    value
                  />
                </div>
            )
          </IfOption>
        | Some("testing_vote")
        | Some("promotion_vote") =>
          <div className=DashboardStyles.rightMdMainContainer>
            <p>
              <IfOption validator={block.active_proposal}>
                (
                  value =>
                    <TextWithCopy
                      className={DashboardStyles.content2(theme)}
                      value
                    />
                )
              </IfOption>
              <IfOption validator={gp.yayRolls}>
                (
                  yayRolls =>
                    <>
                      <span className={DashboardStyles.content3(theme)}>
                        {
                          intl->Intl.formatNumber(float_of_int(yayRolls))
                          |> str
                        }
                      </span>
                      {" rolls have been cast for, " |> str}
                    </>
                )
              </IfOption>
              <IfOption validator={gp.nayRolls}>
                (
                  nayRolls =>
                    <>
                      <span className={DashboardStyles.content3(theme)}>
                        {
                          intl->Intl.formatNumber(float_of_int(nayRolls))
                          |> str
                        }
                      </span>
                      {" against" |> str}
                    </>
                )
              </IfOption>
              <IfOption validator={gp.passRolls}>
                (
                  passRolls =>
                    <>
                      {" and " |> str}
                      <span className={DashboardStyles.content3(theme)}>
                        {
                          intl->Intl.formatNumber(float_of_int(passRolls))
                          |> str
                        }
                      </span>
                      {" have passed." |> str}
                    </>
                )
              </IfOption>
              <IfOption validator=percentYay>
                (
                  value =>
                    <>
                      <span className={DashboardStyles.content3(theme)}>
                        {Js.Float.toString(value) ++ "%" |> str}
                      </span>
                      {
                        " of commited rolls were in favor of the proposal, "
                        |> str
                      }
                      <If validator={value < 90.0}> {"not " |> str} </If>
                      {
                        "exceeding the supermajority requirement of 80%." |> str
                      }
                    </>
                )
              </IfOption>
            </p>
          </div>
        | _ => ReasonReact.null
        }
      }
    </div>
  </>;
};