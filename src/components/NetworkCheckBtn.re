module Styles = {
    open Css;
    let mainContainer = style([
      width(px(25)),
      height(px(25)),
      position(`relative),
      verticalAlign(`middle),
      marginRight(px(20)),
      display(`inlineBlock),
      cursor(`pointer)
    ]);
    
    let container1 = (bgCol, fontCol) => style([
      width(px(20)),
      height(px(20)),
      backgroundColor(hex(bgCol)),
      position(`absolute),
      top(px(0)),
      left(px(0)),
      zIndex(2), 
      color(hex(fontCol)), 
      textAlign(center), 
      lineHeight(px(20)), 
      fontFamily(`custom("monospace")),
    ]);
    let container2 = (shadowCol) => style([
      width(px(20)),
      height(px(20)),
      backgroundColor(hex(shadowCol)),
      border(px(2), solid, black),
      position(`absolute),
      bottom(px(0)),
      right(px(0)),
      zIndex(1)
    ]);
  }
  [@react.component]
  let make = (~bgCol, ~shadowCol, ~fontCol, ~isChecked: bool, ~onClick) => {
    <div className=Styles.mainContainer onClick={onClick}>
      <div className=Styles.container1(bgCol, fontCol)>
        {ReasonReact.string(isChecked ? "x":"")}
      </div>
      <div className=Styles.container2(shadowCol)></div>
    </div>
  }