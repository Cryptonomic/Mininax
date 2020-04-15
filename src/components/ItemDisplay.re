open Themes;
open MainType;
open ReactIntl;
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
      let kind =
        switch (Js.Dict.get(items, "kind")) {
        | None => ""
        | Some(value) => value
        };
      Utils.getFields(~entity, ~kind, ());
    | _ => Utils.getFields(~entity, ())
    };
  let fieldsLength = ref(fields |> Js.Array.length);

  <div className=Styles.mainContainer>
    {fields
     |> Array.mapi((index, field) => {
          let fieldVal =
            switch (Js.Dict.get(items, field.name)) {
            | None => ""
            | Some(value) => value
            };
          ();
        })
     |> ReasonReact.array}
  </div>;
};