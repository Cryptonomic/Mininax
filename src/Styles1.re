open Themes;
open Css;
let container = index => style([
  width(pct(100.)),
  paddingLeft(px(11)),
  position(relative),
  before([
    contentRule(""),
    position(absolute),
    width(px(2)),
    height(Calc.(pct(100.) - px(29))),
    backgroundColor(hex(themes[index].fieldBgColor)),
    left(px(0)),
    top(px(16))
  ])
]);
let titleLinkCss = index => [
  position(relative),
  height(px(31)),
  padding4(~top=px(6), ~bottom=px(6), ~left=px(9), ~right=px(9)),
  fontSize(px(20)),
  color(black),
  backgroundColor(white),
  boxShadows([
    boxShadow(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.2)),
    boxShadow(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.14)),
    boxShadow(~x=px(7), ~y=px(7), ~blur=px(0), ~spread=px(0), hex(themes[index].fieldBgColor)),
  ]),
  before([
    contentRule(""),
    position(absolute),
    width(px(8)),
    height(px(2)),
    backgroundColor(hex(themes[index].fieldBgColor)),
    left(px(-9)),
    top(px(16))
  ])
];

let title = index => style([
  textTransform(capitalize),
  width(px(215)),
  ...titleLinkCss(index)
]);
let linkBtn = index => style([
  width(px(215)),
  cursor(`pointer),
  marginTop(px(10)),
  ...titleLinkCss(index)
]);

let accountBtn = index => style([
  width(px(485)),
  cursor(`pointer),
  marginTop(px(15)),
  ...titleLinkCss(index)
]);

let titleContainer = style([
  display(flexBox),
  alignItems(center)
]);

let titleBtnContainer = style([
  marginLeft(px(30))
]);

let transactionBtn = (index, disabled) => style([
  width(px(31)),
  height(px(31)),
  fontSize(px(20)),
  border(px(0), none, transparent),
  outline(px(0), none, transparent),
  color(black),
  backgroundColor(hex("EEFF00")),
  boxShadows([
    boxShadow(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.2)),
    boxShadow(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.14)),
    boxShadow(~x=px(7), ~y=px(7), ~blur=px(0), ~spread=px(0), hex(themes[index].fieldBgColor)),
  ]),
  marginRight(px(7)),
  opacity(disabled?0.7:1.)
]);
