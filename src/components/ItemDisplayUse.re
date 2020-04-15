open Themes;
open ReactIntl;
open Utils;
open Helpers;
open BsRaw;

module Style = {
  open Css;

  let rowContainer =
    style([
      display(flexBox),
      alignItems(center),
      marginBottom(px(10)),
      lastChild([marginBottom(px(0))]),
    ]);

  let levelLabelContainer =
    style([
      display(flexBox),
      width(px(215)),
      height(px(31)),
      justifyContent(spaceBetween),
    ]);

  let labelCss = index => [
    position(relative),
    height(px(31)),
    padding4(~top=px(7), ~bottom=px(6), ~left=px(9), ~right=px(9)),
    fontSize(px(20)),
    color(hex(themes[index].fieldColor)),
    backgroundColor(hex(themes[index].fieldBgColor)),
  ];

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

  let levelLabel = index =>
    style([width(px(125)), textAlign(center), ...labelCss(index)]);

  let fieldLabel = (isBar, index) =>
    style([
      display(flexBox),
      justifyContent(spaceBetween),
      width(px(215)),
      isBar
        ? before([
            contentRule(`none),
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

  let copyImg = style([cursor(`pointer)]);

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
};

module Make = (()) => {
  let intl = ReactIntl.useIntl();
  let theme = React.useContext(ContextProvider.themeContext);
  let fieldFactory = (~entity, ~items, ~changeLevel, ~goToDetail) => {
    let fields =
      switch (entity) {
      | "operation" =>
        let kind = getStringFieldValue(Js.Dict.get(items), "kind");
        Utils.getFields(~entity, ~kind, ());
      | _ => Utils.getFields(~entity, ())
      };
    let fieldsLength = Array.length(fields);
    let componentsArray =
      fields
      |> Array.mapi((index, field: MainType.field) => {
           let getValue = Js.Dict.get(items) |> getStringFieldValue;
           let fieldVal = field.name |> getValue;
           switch (field.name, field.isLink, fieldVal, field.showNotifierLink) {
           | (_, _, "", _) => ReasonReact.null
           | ("level", _, _, _) =>
             let metaCycle = "meta_cycle" |> getValue;
             let period = "meta_voting_period" |> getValue;
             let levelVal = period ++ " / " ++ metaCycle ++ " / " ++ fieldVal;
             <div className=Style.rowContainer key={field.name}>
               <div className=Style.levelLabelContainer>
                 <button
                   className={Style.levelBtn(theme)}
                   onClick={_ => changeLevel(int_of_string(fieldVal) - 1)}>
                   {ReasonReact.string("<")}
                 </button>
                 <div className={Style.levelLabel(theme)}>
                   {ReasonReact.string("Level")}
                 </div>
                 <button
                   className={Style.levelBtn(theme)}
                   onClick={_ => changeLevel(int_of_string(fieldVal) + 1)}>
                   {ReasonReact.string(">")}
                 </button>
               </div>
               <div className={Style.fieldContent(theme)}>
                 {ReasonReact.string(levelVal)}
               </div>
             </div>;
           | ("script", _, _, _)
           | ("storage", _, _, _)
           | ("parameters", _, _, _) =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
                 <img
                   className=Style.copyImg
                   src=copyYellow
                   onClick={_ => BsRaw.copyContent(fieldVal)}
                 />
               </div>
               <div className={Style.fieldContent(theme)}>
                 {ReasonReact.string(fieldVal)}
               </div>
             </div>
           | ("timestamp", _, _, _) =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
               </div>
               <div className={Style.fieldContent(theme)}>
                 {fieldVal != ""
                    ? intl
                      ->Intl.formatDateWithOptions(
                          Js.Date.fromFloat(fieldVal |> float_of_string),
                          dateTimeFormatOptions,
                        )
                      ->React.string
                    : ReasonReact.string(fieldVal)}
               </div>
             </div>
           | (_, true, _, true) =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
               </div>
               <div
                 className={Style.linkContent(theme)}
                 onClick={_ => goToDetail(fieldVal)}>
                 {ReasonReact.string(fieldVal)}
               </div>
               <div onClick={_ => openTelegramBot(fieldVal)}>
                 <BellSvg />
               </div>
             </div>
           | (_, false, _, true) =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
               </div>
               <div className={Style.fieldContent(theme)}>
                 {ReasonReact.string(fieldVal)}
                 <span onClick={_ => openTelegramBot(fieldVal)}>
                   <BellSvg />
                 </span>
               </div>
             </div>
           | (_, true, _, false) =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength^ - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
               </div>
               <div
                 className={Style.linkContent(theme)}
                 onClick={_ => goToDetail(fieldVal)}>
                 {ReasonReact.string(fieldVal)}
               </div>
             </div>
           | _ =>
             <div className=Style.rowContainer key={field.name}>
               <div
                 className={Style.fieldLabel(
                   index === 0 || index === fieldsLength^ - 1,
                   theme,
                 )}>
                 {ReasonReact.string(field.displayName)}
               </div>
               <div className={Style.fieldContent(theme)}>
                 {ReasonReact.string(fieldVal)}
               </div>
             </div>
           };
         });
    ();
  };
};