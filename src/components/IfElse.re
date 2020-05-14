[@react.component]
let make =
    (
      ~children: (ReasonReact.reactElement, ReasonReact.reactElement),
      ~validator,
    ) => {
  let (firstChild, secondChild) = children;
  validator ? firstChild : secondChild;
};