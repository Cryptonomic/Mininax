open Themes;

open Css;
let container = index =>
  style([
    width(pct(100.)),
    minHeight(vh(100.)),
    display(flexBox),
    flexDirection(column),
    backgroundColor(hex(themes[index].mainBgColor)),
  ]);
let header =
  style([
    padding4(~top=px(45), ~bottom=px(30), ~left=px(45), ~right=px(45)),
  ]);
let headerTitle =
  style([
    fontFamily("'Arcade', sans-serif"),
    fontSize(px(60)),
    color(white),
    lineHeight(px(35)),
    letterSpacing(px(3)),
    cursor(`pointer),
    display(inlineBlock),
  ]);
let mainContainer =
  style([
    padding4(~top=px(0), ~bottom=px(0), ~left=px(35), ~right=px(35)),
    flex(`num(1.)),
    marginBottom(px(40)),
  ]);