[@react.component]
let make =
    (
      ~children: ('a => ReasonReact.reactElement, ReasonReact.reactElement),
      ~validator,
    ) => {
  let (firstChild, secondChild) = children;
  switch (validator) {
  | Some(value) => firstChild(value)
  | None => secondChild
  };
};