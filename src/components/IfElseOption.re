[@react.component]
let make =
    (
      ~children: (ReasonReact.reactElement, ReasonReact.reactElement),
      ~validator,
    ) => {
  let (firstChild, secondChild) = children;
  switch (validator) {
  | Some(_) => firstChild
  | None => secondChild
  };
};