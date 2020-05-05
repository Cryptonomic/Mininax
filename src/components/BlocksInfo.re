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

  <div className={leftTopContainer(theme)}>
    <p>
      {"Greetings! The Tezos " |> str}
      <span className={networkContent(theme)}> {network |> str} </span>
      <IfOption validator={block.meta_cycle}>
        {" is now in cycle " |> str}
        <span className={content1(theme)}>
          {intl
           ->Intl.formatNumber(
               float_of_int(Helpers.optionToInt(block.meta_cycle)),
             )
           ->str}
        </span>
        {"." |> str}
      </IfOption>
    </p>
    <IfOption validator={blockinfo.blockCount}>
      <p>
        {"Within this cycle, " |> str}
        <span className={content1(theme)}>
          {intl->Intl.formatNumber(
             float_of_int(Helpers.optionToInt(blockinfo.blockCount)),
           )
           ++ " of "
           ++ intl->Intl.formatNumber(float_of_int(blocksPerCycle))
           |> str}
        </span>
        {" blocks " |> str}
        <span className={content1(theme)}>
          {"("
           ++ percentBaked(Helpers.optionToInt(blockinfo.blockCount))
           ++ "%)"
           |> str}
        </span>
        {" have been baked." |> str}
      </p>
    </IfOption>
    <IfOption validator={blockinfo.bakersWithOutput}>
      <p className=inline>
        {" In the past day, there have been " |> str}
        <span className={content1(theme)}>
          {intl->Intl.formatNumber(
             float_of_int(Helpers.optionToInt(blockinfo.bakersWithOutput)),
           )
           |> str}
        </span>
        {" bakers who've made block and" |> str}
      </p>
    </IfOption>
    <IfOption validator={blockinfo.zeroPriorityBlocks}>
      <p>
        <span className={content1(theme)}>
          {intl->Intl.formatNumber(
             float_of_int(Helpers.optionToInt(blockinfo.zeroPriorityBlocks)),
           )
           |> str}
        </span>
        {" zero priority blocks." |> str}
      </p>
    </IfOption>
    <IfOption validator={block.hash}>
      <p className=inline> {"The latest block " |> str} </p>
      <TextWithCopy
        className={content1(theme)}
        value={Helpers.optionToString(block.hash)}
        isReverse=true
      />
      <IfOption validator={block.level}>
        <p className=inline> {" at level " |> str} </p>
        <span className={content1(theme)}>
          {intl
           ->Intl.formatNumber(
               float_of_int(Helpers.optionToInt(block.level)),
             )
           ->str}
        </span>
      </IfOption>
      <IfOption validator={block.baker}>
        <p className=inline> {" was baked by " |> str} </p>
        <TextWithCopy
          className={content1(theme)}
          value={Helpers.optionToString(block.baker)}
          isReverse=true
        />
      </IfOption>
      <IfOption validator=latestBlockTime>
        <p className=inline> {" at " |> str} </p>
        <span className={content1(theme)}>
          {latestBlockTime |> Helpers.optionToString |> str}
        </span>
      </IfOption>
      <IfOption validator=latestBlockDate>
        <p className=inline> {" on " |> str} </p>
        <span className={content1(theme)}>
          {latestBlockDate |> Helpers.optionToString |> str}
        </span>
      </IfOption>
      <p className=inline> {"." |> str} </p>
    </IfOption>
  </div>;
};