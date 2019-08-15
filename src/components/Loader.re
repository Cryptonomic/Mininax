[@bs.module] external loadingSvg : string = "../images/loading.svg";
module Styles = {
  open Css;
  let container = style([
    position(fixed),
    display(flexBox),
    alignItems(center),
    justifyContent(center),
    backgroundColor(rgba(0, 0, 0, 0.3)),
    top(px(0)),
    left(px(0)),
    width(pct(100.)),
    height(vh(100.)),
    zIndex(100)
  ]);
};
[@react.component]
let make = () => 
  <div className=Styles.container>
    <img src={loadingSvg} />
  </div>
