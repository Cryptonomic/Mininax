open Themes;
[@bs.module] external searchSvg : string = "../assets/images/search.svg";
[@bs.module] external cryptoLogo : string = "../assets/images/cryptonomic.svg";
[@bs.val] external scrollTo : (float, float) => unit = "scrollTo";

module Styles = {
  open Css;
  let container = style([
    display(flexBox),
    padding4(~top=px(0), ~bottom=px(0), ~left=px(10), ~right=px(10)),
    alignItems(center)
  ]);

  let input = index => style([
    height(px(60)),
    width(px(800)),
    borderRadius(px(10)),
    padding4(~top=px(20), ~bottom=px(20), ~left=px(24), ~right=px(24)),
    color(black),
    fontSize(px(18)),
    outline(px(0), none, transparent),
    boxShadows([
      Shadow.box(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.2)),
      Shadow.box(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.14)),
      Shadow.box(~x=px(7), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.12)),
    ]),
    border(px(1), solid, rgba(0, 0, 0, 0.12)),
    fontFamily(`custom("'Perfect DOS VGA 437 Win', sans-serif")),
    backgroundColor(hex(themes[index].searchBgColor))
  ]);

  let searhBtn = index => style([
    height(px(60)),
    width(px(60)),
    marginLeft(px(15)),
    borderRadius(px(10)),
    outline(px(0), none, transparent),
    backgroundColor(hex(themes[index].searchBgColor)), 
    borderWidth(px(0))
  ]);
  let cryptoImg = style([
    marginLeft(px(35)),
    width(px(55)),
    height(px(55)),
    cursor(`pointer)
  ]);
  let changeBtn = index => style([
    textTransform(uppercase),
    marginLeft(auto),
    padding4(~top=px(0), ~bottom=px(0), ~left=px(10), ~right=px(10)),
    lineHeight(px(23)),
    height(px(31)),
    fontSize(px(18)),
    color(hex(themes[index].mainBgColor)),
    backgroundColor(white),
    boxShadows([
      Shadow.box(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.2)),
      Shadow.box(~x=px(0), ~y=px(4), ~blur=px(12), ~spread=px(0), rgba(0, 0, 0, 0.14)),
      Shadow.box(~x=px(7), ~y=px(7), ~blur=px(0), ~spread=px(0), hex(themes[index].fieldBgColor)),
    ]),
    fontFamily(`custom("'Perfect DOS VGA 437 Win', sans-serif"))
  ]);

  let upDown = style([
    fontSize(px(20))
  ]);
};

[@react.component]
let make = (~searchVal, ~network, ~setRef, ~changeId, ~onSearch, ~onOpenNetworkSelector) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let (isFirstLoad, setIsFirstLoad) = React.useState(() => false);
  let doEffectWhenRefChanges = (ref) => {
    setRef(ref);
    switch (Js.Nullable.toOption(ref), isFirstLoad) {
    | (Some(el), false) => {
      let elementObj = ReactDOMRe.domElementToObj(el);
      setIsFirstLoad(_ => true);
      ignore(elementObj##focus());
      scrollTo(0., 0.);
    }
    | _ => ignore()
    };
  };

  let openCrypto = (_ev) => {
    Utils.open_("https://cryptonomic.tech/", "_blank");
  };


  <div className=Styles.container>
    <input
      ref={ReactDOMRe.Ref.callbackDomRef(ref =>
        doEffectWhenRefChanges(ref)
      )}
      value={searchVal}
      onChange={
        event => changeId(ReactEvent.Form.target(event)##value)
      }
      onKeyDown={
        event =>
          switch (event |> ReactEvent.Keyboard.which ) {
          | 13 => onSearch()
          | _ => ignore()
          };
      }
      className=Styles.input(theme)
      placeholder="Press 's' and search by block ID, operation ID, account ID or block level"
    />
    <button className=Styles.searhBtn(theme) onClick={_=>onSearch()}>
      <img src=searchSvg />
    </button>
    <button className=Styles.changeBtn(theme) onClick={onOpenNetworkSelector}>
      {ReasonReact.string(network)}
      <span className=Styles.upDown>{ReasonReact.string({js|⇵|js})}</span>
    </button>
    <img className=Styles.cryptoImg src=cryptoLogo onClick={openCrypto} />
  </div>
}
