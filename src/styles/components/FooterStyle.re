open Themes;

open Css;

let container =
  style([
    display(flexBox),
    padding4(~top=px(0), ~bottom=px(45), ~left=px(45), ~right=px(55)),
    alignItems(center),
  ]);

let input = index =>
  style([
    height(px(60)),
    width(px(800)),
    borderRadius(px(10)),
    padding4(~top=px(20), ~bottom=px(20), ~left=px(24), ~right=px(24)),
    color(black),
    fontSize(px(18)),
    outline(px(0), none, transparent),
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
        ~x=px(7),
        ~y=px(4),
        ~blur=px(12),
        ~spread=px(0),
        rgba(0, 0, 0, 0.12),
      ),
    ]),
    border(px(1), solid, rgba(0, 0, 0, 0.12)),
    fontFamily("'Perfect DOS VGA 437 Win', sans-serif"),
    backgroundColor(hex(themes[index].searchBgColor)),
  ]);

let searhBtn = index =>
  style([
    height(px(60)),
    width(px(60)),
    marginLeft(px(15)),
    borderRadius(px(10)),
    outline(px(0), none, transparent),
    backgroundColor(hex(themes[index].searchBgColor)),
  ]);
let cryptoImg =
  style([
    marginLeft(px(35)),
    width(px(55)),
    height(px(55)),
    cursor(`pointer),
  ]);
let changeBtn = index =>
  style([
    textTransform(uppercase),
    marginLeft(auto),
    width(px(185)),
    height(px(31)),
    fontSize(px(18)),
    color(hex(themes[index].mainBgColor)),
    backgroundColor(white),
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
        ~x=px(7),
        ~y=px(7),
        ~blur=px(0),
        ~spread=px(0),
        hex(themes[index].fieldBgColor),
      ),
    ]),
    fontFamily("'Perfect DOS VGA 437 Win', sans-serif"),
  ]);

let upDown = style([fontSize(px(20))]);