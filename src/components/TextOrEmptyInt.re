[@react.component]
let make = (~value: option(int)) =>
  switch (value) {
  | Some(value) => value |> string_of_int |> ReasonReact.string
  | None => ReasonReact.null
  };