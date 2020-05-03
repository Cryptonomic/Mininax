[@react.component]
let make = (~value: option(float)) =>
  switch (value) {
  | Some(value) => value |> Js.Float.toString |> ReasonReact.string
  | None => ReasonReact.null
  };