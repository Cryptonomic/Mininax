[@react.component]
let make = (~value: option(string)) =>
  switch (value) {
  | Some(value) => value |> ReasonReact.string
  | None => ReasonReact.null
  };