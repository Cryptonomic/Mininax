open Themes;
open ReactIntl;
open Utils;
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

[@react.component]
let make =
    (
      ~field: MainType.field,
      ~theme,
      ~changeLevel,
      ~index,
      ~fieldsLength,
      ~goToDetail,
      ~getValueFromItem,
      ~intl,
    ) => {
  let fieldValue = getValueFromItem(field.name);
  let displayValue =
    switch (field.name) {
    | "level" =>
      getValueFromItem("meta_voting_period")
      ++ " / "
      ++ getValueFromItem("meta_voting_period")
      ++ " / "
      ++ fieldValue
    | "timestamp" =>
      intl->Intl.formatDateWithOptions(
        Js.Date.fromFloat(fieldValue |> float_of_string),
        Helpers.dateTimeFormatOptions,
      )

    | _ => fieldValue
    };
  <div className=Style.rowContainer>
    {switch (field.name) {
     | "level" =>
       <div className=Style.levelLabelContainer>
         <button
           className={Style.levelBtn(theme)}
           onClick={_ => changeLevel(int_of_string(fieldValue) - 1)}>
           {ReasonReact.string("<")}
         </button>
         <div className={Style.levelLabel(theme)}>
           {ReasonReact.string("Level")}
         </div>
         <button
           className={Style.levelBtn(theme)}
           onClick={_ => changeLevel(int_of_string(fieldValue) + 1)}>
           {ReasonReact.string(">")}
         </button>
       </div>
     | _ =>
       <div
         className={Style.fieldLabel(
           index === 0 || index === fieldsLength - 1,
           theme,
         )}>
         {ReasonReact.string(field.displayName)}
         <If
           validator={
             ["script", "storage", "parameters"]
             |> List.exists(value => value == field.name)
           }>
           <img
             className=Style.copyImg
             src=BsRaw.copyYellow
             onClick={_ => BsRaw.copyContent(fieldValue)}
           />
         </If>
       </div>
     }}
    <If validator={field.isLink}>
      <div
        className={Style.linkContent(theme)}
        onClick={_ => goToDetail(fieldValue)}>
        {ReasonReact.string(displayValue)}
      </div>
    </If>
    <If validator={field.isLink == false}>
      <div className={Style.fieldContent(theme)}>
        {ReasonReact.string(displayValue)}
      </div>
    </If>
    <If validator={field.showNotifierLink}>
      <span onClick={_ => openTelegramBot(fieldValue)}> <BellSvg /> </span>
    </If>
  </div>;
};