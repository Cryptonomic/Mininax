[@react.component]
let make = (~children:ReasonReact.reactElement, ~validator) => {
  switch (validator) {
  | true => children
  | false => ReasonReact.null
  };
}