[@react.component]
let make = (~children, ~validator) => {
  switch (validator) {
  | Some(value) => children(value)
  | None => ReasonReact.null
  };
};