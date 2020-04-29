open Configs;
open MomentRe;
open GlobalStore;
open ReactIntl;
[@bs.module] external searchSvg: string = "../assets/images/search.svg";

let numFormatOptions = numberFormatOptions(~maximumFractionDigits=2, ());

let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.lastBlock,
  state.dashboardState.blockinfo,
  state.dashboardState.transinfo,
  state.dashboardState.voteinfo,
  state.dashboardState.proposalsInfo,
);

[@react.component]
let make = (~onSearch, ~changeLevel) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let intl = ReactIntl.useIntl();
  let (searchVal, setSearchVal) = React.useState(() => "");
  let network = configs[theme].network;
  let blocksPerCycle = Utils.getBlocksPerCycle(network);
  let (items, blockinfo, transinfo, voteinfo, proposals) =
    Store.useSelector(selector);

  let percentBaked =
    Js.Math.round(
      float_of_int(blockinfo.blockCount)
      /. float_of_int(blocksPerCycle)
      *. 100.0,
    );
  let secTimestamp = Utils.getSecondTimeFromMilli(items##timestamp);
  let latestBlockDate =
    momentWithUnix(secTimestamp) |> Moment.format("MMM DD, YYYY");
  let latestBlockTime =
    momentWithUnix(secTimestamp) |> Moment.format("HH:mm");
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
  let periodKind = items##period_kind;

  let sumYayNay = float_of_int(voteinfo.yay_rolls + voteinfo.nay_rolls);
  let percentYay =
    Js.Math.round(float_of_int(voteinfo.yay_rolls) /. sumYayNay *. 100.0);

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

  let gotoLastBlock = () => {
    let url = Utils.makeUrl(~network, ~entity="blocks", ~id=items##hash);
    ReasonReactRouter.push(url);
  };

  let gotoLastOperations = () => {
    let query = Queries.getQueryForBlockLink(items##hash);
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "operations");
  };

  let gotoBlocksArronax = () => {
    let query = Queries.getQueryForBlocksTab();
    let displayName = Utils.getDisplayName(configs[theme]);
    Utils.openSharedUrl(query, displayName, "blocks");
  };

  let onSearchBlock = () =>
    if (searchVal != "") {
      onSearch(searchVal);
    };

  <div className=DashboardStyles.container>
    <div className=DashboardStyles.mainContainer>
      // Block info

        <div className=DashboardStyles.leftContainer>
          <div className={DashboardStyles.leftTopContainer(theme)}>
            {ReasonReact.string("Greetings! The Tezos ")}
            <div className={DashboardStyles.networkContent(theme)}>
              {ReasonReact.string(network)}
            </div>
            {ReasonReact.string(" is now in cycle ")}
            <div className={DashboardStyles.content1(theme)}>
              {intl
               ->Intl.formatNumber(float_of_int(items##meta_cycle))
               ->React.string}
            </div>
            <br />
            {ReasonReact.string("Within this cycle, ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(
                 intl->Intl.formatNumber(float_of_int(blockinfo.blockCount))
                 ++ " of "
                 ++ intl->Intl.formatNumber(float_of_int(blocksPerCycle)),
               )}
            </div>
            {ReasonReact.string(" blocks ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(
                 "(" ++ Js.Float.toString(percentBaked) ++ "%)",
               )}
            </div>
            {ReasonReact.string(" have been baked.")}
            <br />
            {ReasonReact.string("The latest block ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(items##hash)}
              <CopyContent isReverse=true hash=items##hash />
            </div>
            {ReasonReact.string(" at level ")}
            <div className={DashboardStyles.content1(theme)}>
              {intl
               ->Intl.formatNumber(float_of_int(items##level))
               ->React.string}
            </div>
            {ReasonReact.string(" was baked by ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(items##baker)}
              <CopyContent isReverse=true hash=items##baker />
            </div>
            {ReasonReact.string(" at ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(latestBlockTime)}
            </div>
            {ReasonReact.string(" on ")}
            <div className={DashboardStyles.content1(theme)}>
              {ReasonReact.string(latestBlockDate)}
            </div>
            {ReasonReact.string(".")}
          </div>
          <div className={DashboardStyles.leftBottomContainer(theme)}>
            {ReasonReact.string("In the past day there have been ")}
            <div className={DashboardStyles.content3(theme)}>
              {intl
               ->Intl.formatNumber(float_of_string(transinfo.countAmount))
               ->React.string}
            </div>
            {ReasonReact.string(" transactions for a total of ")}
            <div className={DashboardStyles.content3(theme)}>
              {ReasonReact.string(transactions_total_xtz ++ " XTZ")}
            </div>
            {ReasonReact.string(" while ")}
            <div className={DashboardStyles.content3(theme)}>
              {intl
               ->Intl.formatNumber(
                   float_of_string(transinfo.countOriginatedContracts),
                 )
               ->React.string}
            </div>
            {ReasonReact.string(" accounts were originated and ")}
            <div className={DashboardStyles.content3(theme)}>
              {intl
               ->Intl.formatNumber(
                   float_of_string(blockinfo.fundraiserCount),
                 )
               ->React.string}
            </div>
            {ReasonReact.string(
               " fundraiser accounts were activated. A total of ",
             )}
            <div className={DashboardStyles.content3(theme)}>
              {ReasonReact.string(sumFee ++ " XTZ")}
            </div>
            {ReasonReact.string(" in fees have been paid out and ")}
            <div className={DashboardStyles.content3(theme)}>
              {ReasonReact.string(consumedGas)}
            </div>
            {ReasonReact.string(" gas has been consumed. There have been ")}
            <div className={DashboardStyles.content3(theme)}>
              {ReasonReact.string(
                 intl->Intl.formatNumber(
                   float_of_string(blockinfo.totalFundraiserCount),
                 )
                 ++ " of 30,317",
               )}
            </div>
            {ReasonReact.string(" fundraiser accounts ")}
            <div className={DashboardStyles.content2(theme)}>
              {ReasonReact.string(
                 "(" ++ Js.Float.toString(fundraiserPercent) ++ "%)",
               )}
            </div>
            {ReasonReact.string("  activated so far.")}
          </div>
        </div>
        <div className=DashboardStyles.rightContainer>
          // governance process

            <div className={DashboardStyles.rightTopContainer(theme)}>
              {ReasonReact.string("We are currently in the ")}
              <div className={DashboardStyles.networkContent(theme)}>
                {ReasonReact.string(rightTitle)}
              </div>
              {ReasonReact.string(" phase of the governance process.")}
            </div>
            <div className={DashboardStyles.rightMdContainer(theme)}>
              <div> {ReasonReact.string(proposalsTitle)} </div>
              {switch (periodKind) {
               | "proposal" =>
                 <div className=DashboardStyles.rightMdMainContainer>
                   {proposals
                    |> Array.mapi((index, pr: DashboardStore.proposalInfo) => {
                         let lastTxt =
                           if (index === Js.Array.length(proposals) - 1) {
                             " votes.";
                           } else {
                             " votes and ";
                           };
                         <div key={string_of_int(index)}>
                           <div className={DashboardStyles.content2(theme)}>
                             {ReasonReact.string(pr.proposal)}
                             <CopyContent isReverse=false hash={pr.proposal} />
                           </div>
                           <br />
                           {ReasonReact.string("with ")}
                           <div className={DashboardStyles.content3(theme)}>
                             {intl
                              ->Intl.formatNumber(
                                  float_of_string(
                                    pr.count_operation_group_hash,
                                  ),
                                )
                              ->React.string}
                           </div>
                           {ReasonReact.string(lastTxt)}
                         </div>;
                       })
                    |> ReasonReact.array}
                 </div>
               | "testing" =>
                 <div className=DashboardStyles.rightMdMainContainer>
                   <div className={DashboardStyles.content2(theme)}>
                     {ReasonReact.string(items##active_proposal)}
                     <CopyContent
                       isReverse=false
                       hash=items##active_proposal
                     />
                   </div>
                 </div>
               | "testing_vote"
               | "promotion_vote" =>
                 <div className=DashboardStyles.rightMdMainContainer>
                   <div className={DashboardStyles.content2(theme)}>
                     {ReasonReact.string(items##active_proposal)}
                     <CopyContent
                       isReverse=false
                       hash=items##active_proposal
                     />
                   </div>
                   <br />
                   <div className={DashboardStyles.content3(theme)}>
                     {intl
                      ->Intl.formatNumber(float_of_int(voteinfo.yay_rolls))
                      ->React.string}
                   </div>
                   {ReasonReact.string(" rolls have been cast for, ")}
                   <div className={DashboardStyles.content3(theme)}>
                     {intl
                      ->Intl.formatNumber(float_of_int(voteinfo.nay_rolls))
                      ->React.string}
                   </div>
                   {ReasonReact.string(" against and ")}
                   <div className={DashboardStyles.content3(theme)}>
                     {intl
                      ->Intl.formatNumber(float_of_int(voteinfo.pass_rolls))
                      ->React.string}
                   </div>
                   {ReasonReact.string(" have passed.")}
                   <div className={DashboardStyles.content3(theme)}>
                     {ReasonReact.string(
                        Js.Float.toString(percentYay) ++ "%",
                      )}
                   </div>
                   {ReasonReact.string(
                      " of commited rolls were in favor of the proposal, ",
                    )}
                   {percentYay < 90.0
                      ? ReasonReact.string("not ") : ReasonReact.null}
                   {ReasonReact.string(
                      "exceeding the supermajority requirement of 80%.",
                    )}
                 </div>
               | _ => ReasonReact.null
               }}
            </div>
            // All Tezos / bakers
            <div className={DashboardStyles.rightBottomContainer(theme)}>
              /*{ReasonReact.string("There are ")}
                <div className={DashboardStyles.networkContent(theme)}>
                  {intl
                   ->Intl.formatNumber(float_of_string(blockinfo.num_bakers))
                   ->React.string}
                </div>
                {ReasonReact.string(" active bakers. A total of ")}*/

                {ReasonReact.string("A total of ")}
                <div className={DashboardStyles.networkContent(theme)}>
                  {ReasonReact.string(tez_staked ++ " XTZ")}
                </div>
                {ReasonReact.string(" out of ")}
                <div className={DashboardStyles.networkContent(theme)}>
                  {ReasonReact.string(total_tez ++ " XTZ")}
                </div>
                {ReasonReact.string(" or ")}
                <div className={DashboardStyles.networkContent(theme)}>
                  {ReasonReact.string(
                     "(" ++ Js.Float.toString(percent_staked) ++ "%)",
                   )}
                </div>
                {ReasonReact.string(" of TEZ, is being staked right now.")}
              </div>
            <div className=DashboardStyles.rightSearchConainer>
              <div className=DashboardStyles.levelLabelContainer>
                <button
                  className={DashboardStyles.levelBtn(theme)}
                  onClick={_ => changeLevel(items##level - 1)}>
                  {ReasonReact.string("<")}
                </button>
                <div className={DashboardStyles.levelLabel(theme)}>
                  {intl
                   ->Intl.formatNumber(float_of_int(items##level))
                   ->React.string}
                </div>
                <button
                  className={DashboardStyles.levelBtn(theme)}
                  onClick={_ => changeLevel(items##level + 1)}>
                  {ReasonReact.string(">")}
                </button>
              </div>
              <input
                value=searchVal
                onChange={event =>
                  setSearchVal(ReactEvent.Form.target(event)##value)
                }
                onKeyDown={event =>
                  switch (event |> ReactEvent.Keyboard.which) {
                  | 13 => onSearchBlock()
                  | _ => ignore()
                  }
                }
                className={DashboardStyles.input(theme)}
                placeholder="Show stats for block ID or level"
              />
              <button
                className={DashboardStyles.searhBtn(theme)}
                onClick={_ => onSearchBlock()}>
                <img src=searchSvg />
              </button>
            </div>
          </div>
      </div>
    <div className=DashboardStyles.footContainer>
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoLastBlock()}>
        {ReasonReact.string("Block Head")}
      </div>
      <div className={DashboardStyles.divider(theme)} />
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoBlocksArronax()}>
        {ReasonReact.string("Latest Blocks")}
      </div>
      <div className={DashboardStyles.divider(theme)} />
      <div
        className={DashboardStyles.itemContainer(theme)}
        onClick={_ => gotoLastOperations()}>
        {ReasonReact.string("Latest Operations")}
      </div>
    </div>
  </div>;
};