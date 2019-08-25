open Themes;

open Css;
let mainContainer =
  style([
    display(flexBox),
    flexDirection(column),
    marginTop(px(20)),
    marginBottom(px(5)),
  ]);

let rowContainer =
  style([
    display(flexBox),
    alignItems(center),
    marginBottom(px(10)),
    lastChild([marginBottom(px(0))]),
  ]);

let labelCss = index => [
  position(relative),
  height(px(31)),
  padding4(~top=px(7), ~bottom=px(6), ~left=px(9), ~right=px(9)),
  fontSize(px(20)),
  color(hex(themes[index].fieldColor)),
  backgroundColor(hex(themes[index].fieldBgColor)),
];

let levelLabel = index =>
  style([width(px(125)), textAlign(center), ...labelCss(index)]);

let fieldLabel = (isBar, index) =>
  style([
    display(flexBox),
    justifyContent(spaceBetween),
    width(px(215)),
    isBar
      ? before([
          contentRule(""),
          position(absolute),
          width(px(8)),
          height(px(2)),
          backgroundColor(hex(themes[index].fieldBgColor)),
          left(px(-9)),
          top(px(16)),
        ])
      : before([]),
    ...labelCss(index),
  ]);

let fieldContent = index =>
  style([
    marginLeft(px(30)),
    color(hex(themes[index].valueColor)),
    overflow(hidden),
    whiteSpace(nowrap),
    textOverflow(ellipsis),
    flex(`num(1.)),
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
    color(hex(themes[index].fieldColor)),
    backgroundColor(hex(themes[index].fieldBgColor)),
  ]);

let linkContent = index =>
  style([
    marginLeft(px(30)),
    color(hex(themes[index].valueColor)),
    overflow(hidden),
    whiteSpace(nowrap),
    textOverflow(ellipsis),
    textDecoration(underline),
    cursor(`pointer),
  ]);

let copyImg = style([cursor(`pointer)]);