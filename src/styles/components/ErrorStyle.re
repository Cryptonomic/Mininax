open Themes;

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
    width(px(445)),
    height(px(240)),
    padding(px(15)),
    backgroundColor(hex("c22c2c")),
    boxShadows([
      boxShadow(
        ~x=px(0),
        ~y=px(4),
        ~blur=px(12),
        ~spread=px(0),
        rgba(0, 0, 0, 0.2),
      ),
      boxShadow(
        ~x=px(0),
        ~y=px(4),
        ~blur=px(12),
        ~spread=px(0),
        rgba(0, 0, 0, 0.14),
      ),
      boxShadow(
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
    border(px(2), solid, white),
  ]);

let contentContainer =
  style([
    width(pct(100.)),
    height(pct(100.)),
    border(px(2), solid, white),
  ]);

let title =
  style([
    position(relative),
    fontSize(px(20)),
    color(white),
    letterSpacing(px(2)),
    display(inlineBlock),
    padding4(~top=px(0), ~bottom=px(0), ~left=px(15), ~right=px(15)),
    top(px(-13)),
    backgroundColor(hex("c22c2c")),
  ]);

let errTxt =
  style([
    fontSize(px(20)),
    margin4(~top=px(35), ~bottom=px(40), ~left=px(0), ~right=px(0)),
  ]);
let okBtn = index =>
  style([
    fontSize(px(20)),
    padding4(~top=px(7), ~bottom=px(7), ~left=px(15), ~right=px(15)),
    minWidth(px(100)),
    backgroundColor(hex("EEFF00")),
    boxShadows([
      boxShadow(
        ~x=px(0),
        ~y=px(4),
        ~blur=px(12),
        ~spread=px(0),
        rgba(0, 0, 0, 0.2),
      ),
      boxShadow(
        ~x=px(0),
        ~y=px(4),
        ~blur=px(12),
        ~spread=px(0),
        rgba(0, 0, 0, 0.14),
      ),
      boxShadow(
        ~x=px(12),
        ~y=px(10),
        ~blur=px(0),
        ~spread=px(0),
        hex(themes[index].fieldBgColor),
      ),
    ]),
    textAlign(center),
    letterSpacing(px(1)),
    fontFamily("'Perfect DOS VGA 437 Win', sans-serif"),
    outline(px(0), none, transparent),
  ]);