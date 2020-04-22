module Styles = {
  open Css;

  let container = style([width(pct(100.))]);

  let footContainer =
    style([
      width(Calc.(-)(pct(50.), px(30))),
      marginTop(px(77)),
      display(flexBox),
      alignItems(`center),
      paddingLeft(px(10)),
      media(
        "(max-width: 1500px)",
        [width(px(725)), marginLeft(`auto), marginRight(`auto)],
      ),
    ]);

  let itemContainer = index =>
    style([
      backgroundColor(hex(Themes.themes[index].searchBgColor)),
      color(hex(Themes.themes[index].fontCol1)),
      width(px(215)),
      padding4(~top=px(7), ~bottom=px(4), ~left=px(13), ~right=px(7)),
      fontSize(px(19)),
      lineHeight(px(20)),
      boxShadow(
        Shadow.box(
          ~x=px(7),
          ~y=px(7),
          ~blur=px(0),
          ~spread=px(0),
          hex(Themes.themes[index].fieldBgColor),
        ),
      ),
      cursor(`pointer),
    ]);

  let divider = index =>
    style([
      backgroundColor(hex(Themes.themes[index].levelBgcol)),
      height(px(2)),
      flex(`num(1.0)),
    ]);

  let mainContainer =
    style([
      width(pct(100.)),
      paddingLeft(px(10)),
      display(flexBox),
      justifyContent(spaceBetween),
      fontSize(px(16)),
      lineHeight(px(48)),
      media(
        "(max-width: 1500px)",
        [flexDirection(`column), alignItems(`center)],
      ),
    ]);

  let leftContainer = style([width(px(725))]);

  let leftTopContainer = index =>
    style([
      padding4(~top=px(11), ~bottom=px(27), ~left=px(30), ~right=px(23)),
      backgroundColor(hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fieldColor)),
    ]);

  let leftBottomContainer = index =>
    style([
      padding4(~top=px(17), ~bottom=px(23), ~left=px(30), ~right=px(23)),
      border(px(2), solid, hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fontCol2)),
    ]);

  let rightContainer =
    style([
      width(px(725)),
      display(flexBox),
      flexDirection(`column),
      marginLeft(px(60)),
      media(
        "(max-width: 1500px)",
        [marginLeft(px(0)), marginTop(px(50))],
      ),
    ]);

  let rightTopContainer = index =>
    style([
      padding4(~top=px(11), ~bottom=px(18), ~left=px(30), ~right=px(23)),
      backgroundColor(hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fieldColor)),
    ]);

  let rightMdContainer = index =>
    style([
      padding4(~top=px(17), ~bottom=px(34), ~left=px(30), ~right=px(23)),
      border(px(2), solid, hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fontCol2)),
    ]);

  let rightMdMainContainer =
    style([paddingLeft(px(34)), minHeight(px(110))]);

  let rightBottomContainer = index =>
    style([
      padding4(~top=px(21), ~bottom=px(44), ~left=px(30), ~right=px(23)),
      backgroundColor(hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fieldColor)),
      marginTop(`auto),
      media("(max-width: 1500px)", [marginTop(px(30))]),
    ]);

  let content1 = index =>
    style([
      backgroundColor(hex(Themes.themes[index].fieldColor)),
      color(hex(Themes.themes[index].fieldBgColor)),
      padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
      display(`inlineBlock),
      lineHeight(px(16)),
      borderRadius(px(1)),
    ]);

  let networkContent = index =>
    style([
      backgroundColor(hex(Themes.themes[index].fieldColor)),
      color(hex(Themes.themes[index].fieldBgColor)),
      padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
      display(`inlineBlock),
      lineHeight(px(16)),
      borderRadius(px(1)),
      textTransform(`uppercase),
    ]);

  let content2 = index =>
    style([
      backgroundColor(hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fieldColor)),
      padding4(~top=px(8), ~bottom=px(7), ~left=px(11), ~right=px(10)),
      display(`inlineBlock),
      lineHeight(px(16)),
      borderRadius(px(1)),
    ]);

  let content3 = index =>
    style([
      backgroundColor(hex(Themes.themes[index].fieldBgColor)),
      color(hex(Themes.themes[index].fieldColor)),
      padding4(~top=px(8), ~bottom=px(7), ~left=px(17), ~right=px(16)),
      display(`inlineBlock),
      lineHeight(px(16)),
      borderRadius(px(1)),
    ]);

  let levelLabelContainer =
    style([
      display(flexBox),
      width(px(215)),
      height(px(31)),
      justifyContent(spaceBetween),
    ]);

  let levelBtn = index =>
    style([
      width(px(31)),
      height(px(31)),
      fontSize(px(20)),
      border(px(0), none, black),
      outline(px(0), none, transparent),
      color(hex(Themes.themes[index].fieldColor)),
      backgroundColor(hex(Themes.themes[index].levelBgcol)),
    ]);

  let rightSearchConainer = style([marginTop(px(23)), display(flexBox)]);

  let levelLabel = index =>
    style([
      height(px(31)),
      width(px(125)),
      marginRight(px(12)),
      marginLeft(px(12)),
      fontSize(px(16)),
      color(hex(Themes.themes[index].fieldColor)),
      backgroundColor(hex(Themes.themes[index].levelBgcol)),
      display(flexBox),
      justifyContent(`center),
      alignItems(`center),
    ]);

  let input = index =>
    style([
      height(px(31)),
      minWidth(px(200)),
      borderRadius(px(10)),
      padding4(~top=px(7), ~bottom=px(5), ~left=px(31), ~right=px(20)),
      color(black),
      fontSize(px(18)),
      outline(px(0), none, transparent),
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
          ~x=px(7),
          ~y=px(4),
          ~blur=px(12),
          ~spread=px(0),
          rgba(0, 0, 0, 0.12),
        ),
      ]),
      border(px(1), solid, rgba(0, 0, 0, 0.12)),
      fontFamily(`custom("'Perfect DOS VGA 437 Win', sans-serif")),
      backgroundColor(hex(Themes.themes[index].searchBgColor)),
      flex(`num(1.0)),
      marginLeft(px(30)),
    ]);

  let searhBtn = index =>
    style([
      height(px(31)),
      width(px(44)),
      marginLeft(px(16)),
      borderRadius(px(10)),
      outline(px(0), none, transparent),
      backgroundColor(hex(Themes.themes[index].searchBgColor)),
      borderWidth(px(0)),
    ]);
};
