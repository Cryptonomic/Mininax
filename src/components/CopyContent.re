module Styles = {
  open Css;
  let mainContainer =
    style([
      width(px(16)),
      height(px(17)),
      position(`relative),
      verticalAlign(`middle),
      marginLeft(px(12)),
      display(`inlineBlock),
      cursor(`pointer),
    ]);

  let container1 = col =>
    style([
      width(px(11)),
      height(px(12)),
      backgroundColor(hex(col)),
      position(`absolute),
      bottom(px(0)),
      left(px(0)),
      zIndex(2),
    ]);
  let container2 = (fieldColor, fieldBgColor) =>
    style([
      width(px(11)),
      height(px(12)),
      backgroundColor(hex(fieldColor)),
      border(px(2), solid, hex(fieldBgColor)),
      position(`absolute),
      top(px(0)),
      right(px(0)),
      zIndex(1),
    ]);
};
[@react.component]
let make = (~isReverse, ~hash) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let (fieldColor, fieldBgColor) =
    switch (isReverse) {
    | true => (
        Themes.themes[theme].fieldColor,
        Themes.themes[theme].fieldBgColor,
      )
    | _ => (
        Themes.themes[theme].fieldBgColor,
        Themes.themes[theme].fieldColor,
      )
    };
  <div className=Styles.mainContainer onClick={_ => BsRaw.copyContent(hash)}>
    <div className={Styles.container1(fieldBgColor)} />
    <div className={Styles.container2(fieldColor, fieldBgColor)} />
  </div>;
};