open Themes;
open MainType;
open ReactIntl;
[@bs.module] external copyYellow : string = "../assets/images/copy-yellow.svg";
module Styles = {
  open Css;
  let mainContainer = style([
    display(flexBox),
    flexDirection(column),
    marginTop(px(20)),
    marginBottom(px(5))
  ]);

  let rowContainer = style([
    display(flexBox),
    alignItems(center),
    marginBottom(px(10)),
    lastChild([
      marginBottom(px(0))
    ])
  ]);

  let labelCss = index => [
    position(relative),
    height(px(31)),
    padding4(~top=px(7), ~bottom=px(6), ~left=px(9), ~right=px(9)),
    fontSize(px(20)),
    color(hex(themes[index].fieldColor)),
    backgroundColor(hex(themes[index].fieldBgColor))
  ];

  let levelLabel = index => style([
    width(px(125)),
    textAlign(center),
    ...labelCss(index)
  ]);

  let fieldLabel = (isBar, index) => style([
    display(flexBox),
    justifyContent(spaceBetween),
    width(px(215)),
    isBar?before([
      contentRule(`none),
      position(absolute),
      width(px(8)),
      height(px(2)),
      backgroundColor(hex(themes[index].fieldBgColor)),
      left(px(-9)),
      top(px(16))
    ]):before([]),
    ...labelCss(index),
  ]);

  let fieldContent = index => style([
    marginLeft(px(30)),
    color(hex(themes[index].valueColor)),
    overflow(hidden),
    whiteSpace(nowrap),
    textOverflow(ellipsis),
    flex(`num(1.))
  ]);

  let levelLabelContainer = style([
    display(flexBox),
    width(px(215)),
    height(px(31)),
    justifyContent(spaceBetween)
  ]);

  let levelBtn = index => style([
    width(px(31)),
    height(px(31)),
    fontSize(px(20)),
    border(px(0), none, black),
    outline(px(0), none, transparent),
    color(hex(themes[index].fieldColor)),
    backgroundColor(hex(themes[index].fieldBgColor))
  ]);

  let linkContent = index => style([
    marginLeft(px(30)),
    color(hex(themes[index].valueColor)),
    overflow(hidden),
    whiteSpace(nowrap),
    textOverflow(ellipsis),
    textDecoration(underline),
    cursor(`pointer)
  ]);

  let copyImg = style([
    cursor(`pointer)
  ]);
};

let dateTimeFormatOptions = dateTimeFormatOptions(~month=`long, ~day=`numeric, ~year=`numeric, ~hour=`numeric, ~minute=`numeric, ~timeZoneName=`short, ());

[@react.component]
let make = (~entity: string, ~items: Js.Dict.t(string), ~changeLevel, ~goToDetail) => {
  let intl = ReactIntl.useIntl();
  let theme = React.useContext(ContextProvider.themeContext);
  let fields = switch entity {
    | "operation" => {
      let kind = switch (Js.Dict.get(items, "kind")) {
        | None => ""
        | Some(value) => value
      };
      Utils.getFields(~entity=entity, ~kind=kind, ());
    }
    | _ => Utils.getFields(~entity=entity, ())
  };
  let fieldsLength = ref(fields |> Js.Array.length);

  let openTelegramBot = (address: string) => {
    let newUrl = "https://t.me/TezosNotifierBot?start=mininax_" ++ address;
    Utils.open_(newUrl, "_blank");
  };
  
  <div className=Styles.mainContainer>
    (
      fields
      |> Array.mapi((index, field) => {
          let fieldVal = switch (Js.Dict.get(items, field.name)) {
            | None => ""
            | Some(value) => value
          };

          switch (field.name, field.isLink, fieldVal, field.showNotifierLink) {
            | (_, _, "", _) => {
              ReasonReact.null;
            }
            | ("level", _, _, _) => {
              let metaCycle = switch (Js.Dict.get(items, "meta_cycle")) {
                | None => ""
                | Some(value) => value
              };
              let period = switch (Js.Dict.get(items, "meta_voting_period")) {
                | None => ""
                | Some(value) => value
              };
              let levelVal = period ++ " / " ++ metaCycle ++ " / " ++ fieldVal;
              (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.levelLabelContainer>
                  <button className=Styles.levelBtn(theme) onClick={_ => changeLevel(int_of_string(fieldVal) - 1)}>(ReasonReact.string("<"))</button>
                  <div className=Styles.levelLabel(theme)>(ReasonReact.string("Level"))</div>
                  <button className=Styles.levelBtn(theme) onClick={_ => changeLevel(int_of_string(fieldVal) + 1)}>(ReasonReact.string(">"))</button>
                </div>
                <div className=Styles.fieldContent(theme)>(ReasonReact.string(levelVal))</div>
              </div>
            )}
            | ("script", _, _, _) | ("storage", _, _, _) | ("parameters", _, _, _) => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                  <img className=Styles.copyImg src={copyYellow} onClick={_=> Utils.copyContent(fieldVal)} />
                </div>
                <div className=Styles.fieldContent(theme)>(ReasonReact.string(fieldVal))</div>
              </div>
            )
            | ("timestamp", _, _, _) => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                </div>
                <div className=Styles.fieldContent(theme)>
                  (fieldVal !="" ? intl->Intl.formatDateWithOptions(Js.Date.fromFloat(fieldVal |> float_of_string), dateTimeFormatOptions)->React.string : ReasonReact.string(fieldVal))
                </div>
              </div>
            )
            | (_, true, _, true) => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                </div>
                <div className=Styles.linkContent(theme) onClick={_ => goToDetail(fieldVal)}>
                  (ReasonReact.string(fieldVal))
                </div>
                <div onClick={_ => openTelegramBot(fieldVal)}>
                  <BellSvg />
                </div>
              </div>
            )
            | (_, false, _, true) => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                </div>
                <div className=Styles.fieldContent(theme)>(ReasonReact.string(fieldVal))
                    <span onClick={_ => openTelegramBot(fieldVal)}>
                        <BellSvg />
                    </span>
                </div>
              </div>
            )
            | (_, true, _, false) => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                </div>
                <div className=Styles.linkContent(theme) onClick={_ => goToDetail(fieldVal)}>(ReasonReact.string(fieldVal))</div>
              </div>
            )
            | _ => (
              <div className=Styles.rowContainer key=field.name>
                <div className=Styles.fieldLabel(index === 0 || index === fieldsLength^ - 1, theme)>
                  (ReasonReact.string(field.displayName))
                </div>
                <div className=Styles.fieldContent(theme)>(ReasonReact.string(fieldVal))</div>
              </div>
            )
          }
        }
      )

      |> ReasonReact.array
    )
  </div>
}
