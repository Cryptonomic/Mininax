open Configs;
open ReactIntl;
open GlobalStore;
open MomentRe;
open DashboardStyles;

let str = ReasonReact.string;
let selector = (state: GlobalStore.globalState) => (
  state.dashboardState.lastBlock,
  state.dashboardState.blocksInfo,
);

[@react.component]
let make = () => {
  let theme = React.useContext(ContextProvider.themeContext);
  let network = configs[theme].network;
  let intl = ReactIntl.useIntl();
  let (block, blockinfo) = Store.useSelector(selector);

  let blocksPerCycle = Utils.getBlocksPerCycle(network);
  let percentBaked = value =>
    Js.Math.round(
      float_of_int(value) /. float_of_int(blocksPerCycle) *. 100.0,
    )
    |> Js.Float.toString;

  let (latestBlockDate, latestBlockTime) =
    switch (block.timestamp) {
    | Some(timestamp) =>
      let secTimestamp = Utils.getSecondTimeFromMilli(timestamp);
      (
        momentWithUnix(secTimestamp)
        |> Moment.format("MMM DD, YYYY")
        |> Helpers.toOption,
        momentWithUnix(secTimestamp)
        |> Moment.format("HH:mm")
        |> Helpers.toOption,
      );
    | None => (None, None)
    };

  let hash =
    switch (block.hash, latestBlockTime, latestBlockDate) {
    | (Some(value), result1, result2) => Some((value, result1, result2))
    | _ => None
    };

  let cyclyAndPeriod =
    switch (block.meta_cycle, block.meta_voting_period) {
    | (Some(meta_cycle), Some(meta_voting_period)) =>
      Some((meta_cycle, meta_voting_period))
    | _ => None
    };

  <div className={leftTopContainer(theme)}>
    <p className=inline>
      {"Greetings! The Tezos " |> str}
      <span className={networkContent(theme)}> {network |> str} </span>
      <IfOption validator=cyclyAndPeriod>
        {((meta_cycle, meta_voting_period)) =>
           <>
             {" is now in cycle " |> str}
             <span className={content1(theme)}>
               {intl->Intl.formatNumber(float_of_int(meta_cycle)) |> str}
             </span>
             {" and " |> str}
             <span className={content1(theme)}>
               {intl->Intl.formatNumber(float_of_int(meta_voting_period))
                |> str}
             </span>
             {" period. " |> str}
           </>}
      </IfOption>
    </p>
    <IfOption validator={blockinfo.blockCount}>
      {value =>
         <p className=inline>
           {"Within this cycle, " |> str}
           <span className={content1(theme)}>
             {intl->Intl.formatNumber(float_of_int(value))
              ++ " of "
              ++ intl->Intl.formatNumber(float_of_int(blocksPerCycle))
              |> str}
           </span>
           {" blocks " |> str}
           <span className={content1(theme)}>
             {"(" ++ percentBaked(value) ++ "%)" |> str}
           </span>
           {" have been baked." |> str}
         </p>}
    </IfOption>
    <IfOption validator={blockinfo.bakersWithOutput}>
      {value =>
         <p className=inline>
           {" In the past day, there have been " |> str}
           <span className={content1(theme)}>
             {intl->Intl.formatNumber(float_of_int(value)) |> str}
           </span>
           {" bakers who've made block and" |> str}
         </p>}
    </IfOption>
    <IfOption validator={blockinfo.zeroPriorityBlocks}>
      {value =>
         <p>
           <span className={content1(theme)}>
             {intl->Intl.formatNumber(float_of_int(value)) |> str}
           </span>
           {" zero priority blocks." |> str}
         </p>}
    </IfOption>
    <IfOption validator=hash>
      {((value, latestBlockTime, latestBlockDate)) =>
         <>
           <p className=inline> {"The latest block " |> str} </p>
           <TextWithCopy className={content1(theme)} value isReverse=true />
           <IfOption validator={block.level}>
             {level =>
                <>
                  <p className=inline> {" at level " |> str} </p>
                  <span className={content1(theme)}>
                    {intl->Intl.formatNumber(float_of_int(level)) |> str}
                  </span>
                </>}
           </IfOption>
           <IfOption validator={block.baker}>
             {baker =>
                <>
                  <p className=inline> {" was baked by " |> str} </p>
                  <TextWithCopy
                    className={content1(theme)}
                    value=baker
                    isReverse=true
                  />
                </>}
           </IfOption>
           <IfOption validator=latestBlockTime>
             {time =>
                <>
                  <p className=inline> {" at " |> str} </p>
                  <span className={content1(theme)}> {time |> str} </span>
                </>}
           </IfOption>
           <IfOption validator=latestBlockDate>
             {date =>
                <>
                  <p className=inline> {" on " |> str} </p>
                  <span className={content1(theme)}> {date |> str} </span>
                </>}
           </IfOption>
           <p className=inline> {"." |> str} </p>
         </>}
    </IfOption>
  </div>;
};