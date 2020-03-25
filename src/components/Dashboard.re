open Configs;
open MomentRe;

module Styles = {
  open Css;

  let container = style([
    width(pct(100.))
  ]);

  let footContainer = style([
    width(Calc.(-)(pct(50.), px(30))),
    marginTop(px(77)), 
    display(flexBox), 
    alignItems(`center), 
    paddingLeft(px(10)), 
    media("(max-width: 1500px)", [
      width(px(725)),
      marginLeft(`auto),
      marginRight(`auto),
    ])
  ]);

  let itemContainer = index => style([
    backgroundColor(hex(Themes.themes[index].fieldColor)),
    color(hex(Themes.themes[index].fieldBgColor)),
    width(px(215)), 
    padding4(~top=px(7), ~bottom=px(4), ~left=px(13), ~right=px(7)),
    fontSize(px(19)), 
    lineHeight(px(20)), 
    boxShadow(Shadow.box(~x=px(7), ~y=px(7), ~blur=px(0), ~spread=px(0), hex(Themes.themes[index].fieldBgColor))),
  ]);

  let divider = index => style([
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    height(px(2)), 
    flex(`num(1.0))
  ]);

  let mainContainer = style([
    width(pct(100.)),
    paddingLeft(px(10)),
    display(flexBox),
    justifyContent(spaceBetween),
    fontSize(px(16)),
    lineHeight(px(48)), 
    media("(max-width: 1500px)", [
      flexDirection(`column), 
      alignItems(`center)
    ])
  ]);

  let leftContainer = style([
    width(px(725))
  ]);

  let leftTopContainer = index => style([
    padding4(~top=px(11), ~bottom=px(27), ~left=px(30), ~right=px(23)),
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor))
  ]);

  let leftBottomContainer = index => style([
    padding4(~top=px(17), ~bottom=px(23), ~left=px(30), ~right=px(23)),
    border(px(2), solid, hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor))
  ]);

  let rightContainer = style([
    width(px(725)),
    display(flexBox), 
    flexDirection(`column), 
    marginLeft(px(60)), 
    media("(max-width: 1500px)", [
      marginLeft(px(0)),
      marginTop(px(50)),
    ])
  ]);

  let rightTopContainer = index => style([
    padding4(~top=px(11), ~bottom=px(18), ~left=px(30), ~right=px(23)),
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor))
  ]);

  let rightMdContainer = index => style([
    padding4(~top=px(17), ~bottom=px(34), ~left=px(30), ~right=px(23)),
    border(px(2), solid, hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor))
  ]);

  let rightMdMainContainer = style([
    paddingLeft(px(34)),
    minHeight(px(110))
  ]);

  let rightBottomContainer = index => style([
    padding4(~top=px(21), ~bottom=px(44), ~left=px(30), ~right=px(23)),
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor)), 
    marginTop(`auto), 
    media("(max-width: 1500px)", [
      marginTop(px(30))
    ])
  ]);

  let content1 = index => style([
    backgroundColor(hex(Themes.themes[index].fieldColor)),
    color(hex(Themes.themes[index].fieldBgColor)),
    padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
    display(`inlineBlock),
    lineHeight(px(16)),
    borderRadius(px(1))
  ]);

  let networkContent = index => style([
    backgroundColor(hex(Themes.themes[index].fieldColor)),
    color(hex(Themes.themes[index].fieldBgColor)),
    padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
    display(`inlineBlock),
    lineHeight(px(16)),
    borderRadius(px(1)),
    textTransform(`uppercase)
  ]);

  let content2 = index => style([
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor)),
    padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
    display(`inlineBlock),
    lineHeight(px(16)),
    borderRadius(px(1))
  ]);

  let content3 = index => style([
    backgroundColor(hex(Themes.themes[index].fieldBgColor)),
    color(hex(Themes.themes[index].fieldColor)),
    padding4(~top=px(8), ~bottom=px(7), ~left=px(17), ~right=px(16)),
    display(`inlineBlock),
    lineHeight(px(16)),
    borderRadius(px(1))
  ]);
};

[@react.component]
let make = (~items, ~blockinfo: MainType.blockInfo, ~transinfo: MainType.transInfo) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let network = configs[theme].network;
  let blocksPerCycle = Utils.getBlocksPerCycle(network);
  let percentBaked = Js.Math.round((float_of_int(blockinfo.blockCount) /. float_of_int(blocksPerCycle)) *. 100.0);
  Js.log2("---items----", items);
  let secTimestamp = Utils.getSecondTimeFromMilli(items##timestamp);
  let latestBlockDate = momentWithUnix(secTimestamp) |> Moment.format("MMM DD, YYYY.");
  let latestBlockTime = momentWithUnix(secTimestamp) |> Moment.format("HH:mm");
  let transactions_total_xtz = Js.Float.toFixedWithPrecision(Utils.convertFromUtezToTez(transinfo.sumAmount), 2);
  let fundraiserPercent = Js.Math.round((float_of_string(blockinfo.totalFundraiserCount) /. 30317.0) *. 100.0);
  let sumFee = Js.Float.toFixedWithPrecision(Utils.convertFromUtezToTez(blockinfo.sum_fee), 2);
  let consumedGas = Js.Float.toFixedWithPrecision(Utils.convertFromUtezToTez(blockinfo.sum_consumed_gas), 2);

  let tez_staked = Js.Float.toFixedWithPrecision(Utils.convertFromUtezfToTez(blockinfo.bakers_sum_staking_balance), 2);
  let total_tez = Js.Float.toFixedWithPrecision(Utils.convertFromUtezfToTez(blockinfo.totalTez), 2);
  let percent_staked = Js.Math.round((blockinfo.bakers_sum_staking_balance /. blockinfo.totalTez) *. 100.0);
  let periodKind = items##period_kind;

  let rightTitle = switch periodKind {
  | "proposal" => "PROPOSAL"
  | "testing"=> "TESTING"
  | "testing_vote" => "EXPLORATION"
  | "promotion_vote" => "PROMOTION VOTE"
  | _ => ""
  };

  <div className=Styles.container>
    <div className=Styles.mainContainer>
      <div className=Styles.leftContainer>
        <div className=Styles.leftTopContainer(theme)>
          {ReasonReact.string("Greetings! The Tezos ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string(network)}</div>
          {ReasonReact.string(" is now in cycle ")}
          <div className=Styles.content1(theme)>{ReasonReact.string(string_of_int(items##meta_cycle))}</div>
          <br />
          {ReasonReact.string("Within this cycle, ")}
          <div className=Styles.content1(theme)>{ReasonReact.string(string_of_int(blockinfo.blockCount) ++ " of " ++ string_of_int(blocksPerCycle))}</div>
          {ReasonReact.string(" blocks ")}
          <div className=Styles.content1(theme)>{ReasonReact.string("(" ++ Js.Float.toString(percentBaked) ++ "%)")}</div>
          {ReasonReact.string(" have been baked.")}
          <br />
          {ReasonReact.string("The latest block ")}
          <div className=Styles.content1(theme)>
            {ReasonReact.string(items##hash)}
            <CopyContent isReverse={true} />
          </div>
          {ReasonReact.string(" at level ")}
          <div className=Styles.content1(theme)>{ReasonReact.string(string_of_int(items##level))}</div>
          {ReasonReact.string(" was baked by ")}
          <div className=Styles.content1(theme)>
            {ReasonReact.string(items##baker)}
            <CopyContent isReverse={true} />
          </div>
          {ReasonReact.string(" at ")}
          <div className=Styles.content1(theme)>{ReasonReact.string(latestBlockTime)}</div>
          {ReasonReact.string(" on ")}
          <div className=Styles.content1(theme)>{ReasonReact.string(latestBlockDate)}</div>
        </div>
        <div className=Styles.leftBottomContainer(theme)>
          {ReasonReact.string("In the past day there have been ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(transinfo.countAmount)}</div>
          {ReasonReact.string(" transactions for a total of ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(transactions_total_xtz ++ " XTZ")}</div>
          {ReasonReact.string(" while ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(transinfo.countOriginatedContracts)}</div>
          {ReasonReact.string(" accounts were originated and ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(blockinfo.fundraiserCount)}</div>
          {ReasonReact.string(" fundraiser accounts were activated. A total of ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(sumFee ++ " XTZ")}</div>
          {ReasonReact.string(" in fees have been paid out and ")}
          <div className=Styles.content3(theme)>{ReasonReact.string(consumedGas)}</div>
          {ReasonReact.string(" gas has been consumed. There have been ")}
          <div className=Styles.content3(theme)>{ReasonReact.string( blockinfo.totalFundraiserCount ++ " of 30,317")}</div>
          {ReasonReact.string(" fundraiser accounts ")}
          <div className=Styles.content2(theme)>{ReasonReact.string("(" ++ Js.Float.toString(fundraiserPercent) ++ "%)")}</div>
          {ReasonReact.string("  activated so far.")}
        </div>
      </div>
      <div className=Styles.rightContainer>
        <div className=Styles.rightTopContainer(theme)>
          {ReasonReact.string("We are currently in the ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string(rightTitle)}</div>
          {ReasonReact.string(" phase of the governance process.")}
        </div>
        <div className=Styles.rightMdContainer(theme)>
          <div>{ReasonReact.string("The active proposals are:")}</div>
          {switch (periodKind) {
            | "proposal" =>
              <div className=Styles.rightMdMainContainer>
                <div className=Styles.content2(theme)>
                  {ReasonReact.string(items##hash)}
                  <CopyContent isReverse={false} />
                </div>
                <br /> 
                {ReasonReact.string("with ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("7674")}</div>
                {ReasonReact.string(" votes and ")}
                <div className=Styles.content2(theme)>
                  {ReasonReact.string(items##hash)}
                  <CopyContent isReverse={false} />
                </div>
                <br /> 
                {ReasonReact.string("with ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("7674")}</div>
                {ReasonReact.string(" votes.")}
              </div>
            | "testing" =>
              <div className=Styles.rightMdMainContainer>
                <div className=Styles.content2(theme)>
                  {ReasonReact.string(items##hash)}
                  <CopyContent isReverse={false} />
                </div>
              </div>
            | "testing_vote" =>
              <div className=Styles.rightMdMainContainer>
                <div className=Styles.content2(theme)>
                  {ReasonReact.string(items##hash)}
                  <CopyContent isReverse={false} />
                </div>
                <br /> 
                <div className=Styles.content3(theme)>{ReasonReact.string("46,390")}</div>
                {ReasonReact.string(" of the required quorum of ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("8,000")}</div>
                {ReasonReact.string(" ballots have been cast.")}
                <br /> 
                <div className=Styles.content3(theme)>{ReasonReact.string("26,841")}</div>
                {ReasonReact.string(" ballots have been cast for, ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("11")}</div>
                {ReasonReact.string(" against and ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("19,538")}</div>
                {ReasonReact.string(" have passed.")}
              </div>
            | "promotion_vote" =>
              <div className=Styles.rightMdMainContainer>
                <div className=Styles.content2(theme)>
                  {ReasonReact.string(items##hash)}
                  <CopyContent isReverse={false} />
                </div>
                <br /> 
                <div className=Styles.content3(theme)>{ReasonReact.string("46,390")}</div>
                {ReasonReact.string(" of the required quorum of ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("8,000")}</div>
                {ReasonReact.string(" ballots have been cast.")}
                <br /> 
                <div className=Styles.content3(theme)>{ReasonReact.string("26,841")}</div>
                {ReasonReact.string(" ballots have been cast for, ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("11")}</div>
                {ReasonReact.string(" against and ")}
                <div className=Styles.content3(theme)>{ReasonReact.string("19,538")}</div>
                {ReasonReact.string(" have passed.")}
              </div>
            | _ => ReasonReact.null
          }}
        </div>
        <div className=Styles.rightBottomContainer(theme)>
          {ReasonReact.string("There are ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string(blockinfo.num_bakers)}</div>
          {ReasonReact.string(" active bakers. A total of ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string(tez_staked ++ " XTZ")}</div>
          {ReasonReact.string(" out of ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string(total_tez ++ " XTZ,")}</div>
          {ReasonReact.string(" ")}
          <div className=Styles.networkContent(theme)>{ReasonReact.string("(" ++ Js.Float.toString(percent_staked) ++ "%)")}</div>
          {ReasonReact.string(" of TEZ, is being staked right now.")}
        </div>
      </div>
    </div>
    <div className=Styles.footContainer>
      <div className=Styles.itemContainer(theme)>
        {ReasonReact.string("Block Head")}
      </div>
      <div className=Styles.divider(theme)></div>
      <div className=Styles.itemContainer(theme)>
        {ReasonReact.string("Latest Blocks")}
      </div>
      <div className=Styles.divider(theme)></div>
      <div className=Styles.itemContainer(theme)>
        {ReasonReact.string("Latest Operations")}
      </div>
    </div>
  </div>
}
