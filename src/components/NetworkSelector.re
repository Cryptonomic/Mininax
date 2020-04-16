open Themes;
open Configs;
open MainType;
module Styles = {
  open Css;
  let container =
    style([
      position(fixed),
      display(flexBox),
      alignItems(center),
      justifyContent(center),
      backgroundColor(rgba(0, 0, 0, 0.3)),
      top(px(0)),
      left(px(0)),
      width(pct(100.)),
      height(vh(100.)),
      zIndex(50),
    ]);
  let mainContainer = index =>
    style([
      width(px(400)),
      height(px(285)),
      padding(px(15)),
      backgroundColor(hex("ffffff")),
      boxShadows([
        Shadow.box(
          ~x=px(0),
          ~y=px(4),
          ~blur=px(12),
          ~spread=px(0),
          rgba(0, 0, 0, 0.2),
        ),
        Shadow.box(
          ~x=px(0),
          ~y=px(4),
          ~blur=px(12),
          ~spread=px(0),
          rgba(0, 0, 0, 0.14),
        ),
        Shadow.box(
          ~x=px(20),
          ~y=px(20),
          ~blur=px(0),
          ~spread=px(0),
          hex(themes[index].fieldBgColor),
        ),
      ]),
      textAlign(center),
    ]);
  let borderContainer =
    style([
      width(pct(100.)),
      height(pct(100.)),
      padding(px(4)),
      border(px(2), solid, black),
    ]);

  let contentContainer =
    style([
      width(pct(100.)),
      height(pct(100.)),
      border(px(2), solid, black),
    ]);

  let title =
    style([
      position(relative),
      fontSize(px(20)),
      color(black),
      letterSpacing(px(2)),
      display(inlineBlock),
      padding4(~top=px(0), ~bottom=px(0), ~left=px(15), ~right=px(15)),
      top(px(-13)),
      backgroundColor(hex("ffffff")),
    ]);

  let okBtn = index =>
    style([
      fontSize(px(20)),
      padding4(~top=px(7), ~bottom=px(7), ~left=px(15), ~right=px(15)),
      minWidth(px(100)),
      backgroundColor(hex("ffffff")),
      boxShadows([
        Shadow.box(
          ~x=px(0),
          ~y=px(4),
          ~blur=px(12),
          ~spread=px(0),
          rgba(0, 0, 0, 0.2),
        ),
        Shadow.box(
          ~x=px(0),
          ~y=px(4),
          ~blur=px(12),
          ~spread=px(0),
          rgba(0, 0, 0, 0.14),
        ),
        Shadow.box(
          ~x=px(12),
          ~y=px(10),
          ~blur=px(0),
          ~spread=px(0),
          hex(themes[index].fieldBgColor),
        ),
      ]),
      textAlign(center),
      letterSpacing(px(1)),
      fontFamily(`custom("'Perfect DOS VGA 437 Win', sans-serif")),
      outline(px(0), none, transparent),
      border(px(2), solid, black),
    ]);

  let selectContainer =
    style([
      padding4(~top=px(30), ~bottom=px(10), ~left=px(48), ~right=px(48)),
    ]);

  let rowcontainer =
    style([
      fontSize(px(20)),
      marginBottom(px(35)),
      display(flexBox),
      alignItems(center),
    ]);

  let checkBtn = index =>
    style([
      marginRight(px(40)),
      width(px(20)),
      height(px(20)),
      fontFamily(`custom("'Perfect DOS VGA 437 Win', sans-serif")),
      boxShadow(
        Shadow.box(
          ~x=px(3),
          ~y=px(3),
          ~blur=px(0),
          ~spread=px(0),
          hex(themes[index].fieldBgColor),
        ),
      ),
      border(px(0), none, transparent),
      outline(px(0), none, transparent),
      fontSize(px(18)),
      backgroundColor(hex(themes[index].networkBgCol)),
      color(hex(themes[index].mainBgColor)),
    ]);
};
[@react.component]
let make = (~selectedIndex, ~onChange, ~onCancel) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let (selected, setSelected) = React.useState(() => selectedIndex);

  let change = (_event, index) => {
    setSelected(_ => index);
    ReactEvent.Mouse.stopPropagation(_event);
  };

  let confirm = _event => {
    onChange(selected);
    ReactEvent.Mouse.stopPropagation(_event);
  };

  let stop = _event => ReactEvent.Mouse.stopPropagation(_event);

  <div className=Styles.container onClick={_ => onCancel()}>
    <div className={Styles.mainContainer(theme)} onClick=stop>
      <div className=Styles.borderContainer>
        <div className=Styles.contentContainer>
          <div className=Styles.title>
            {ReasonReact.string("Select a Network")}
          </div>
          <div className=Styles.selectContainer>
            {configs
             |> Array.mapi((index, config) =>
                  <div key={config.network} className=Styles.rowcontainer>
                    <NetworkCheckBtn
                      bgCol={themes[index].networkBgCol}
                      shadowCol={themes[index].networkShadowCol}
                      fontCol={themes[index].fieldColor}
                      isChecked={selected === index}
                      onClick={_ev => change(_ev, index)}
                    />
                    {ReasonReact.string(config.displayName)}
                  </div>
                )
             |> ReasonReact.array}
          </div>
          <button className={Styles.okBtn(theme)} onClick=confirm>
            {ReasonReact.string("Ok")}
          </button>
        </div>
      </div>
    </div>
  </div>;
};