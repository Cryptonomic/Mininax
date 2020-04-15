open Helpers;
open MainType;
module Styles = {
  open Css;
  let mainContainer =
    style([
      display(flexBox),
      flexDirection(column),
      marginTop(px(20)),
      marginBottom(px(5)),
    ]);
};

[@react.component]
let make =
    (~entity: string, ~items: Js.Dict.t(string), ~changeLevel, ~goToDetail) => {
  let intl = ReactIntl.useIntl();
  let theme = React.useContext(ContextProvider.themeContext);
  let fields =
    switch (entity) {
    | "operation" =>
      let kind = getStringFieldValue(Js.Dict.get(items), "kind");
      Utils.getFields(~entity, ~kind, ());
    | _ => Utils.getFields(~entity, ())
    };
  let fieldsLength = Array.length(fields);
  let getValue = Js.Dict.get(items) |> getStringFieldValue;
  let reactFields =
    fields
    |> Array.mapi((index, field: MainType.field) =>
         switch (getValue(field.name)) {
         | "" => ReasonReact.null
         | _ =>
           <SingleItem
             key={field.name}
             field
             theme
             changeLevel
             index
             fieldsLength
             goToDetail
             getValueFromItem=getValue
             intl
           />
         }
       );

  <div className=Styles.mainContainer>
    {reactFields |> ReasonReact.array}
  </div>;
};