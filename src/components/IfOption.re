[@react.component]
let make = (~children: ReasonReact.reactElement, ~validator) => {
  switch (validator) {
  | Some(_) => children
  | None => ReasonReact.null
  };
};