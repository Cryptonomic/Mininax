[@react.component]
let make = (~view, ~id, ~network) =>
  <div>
    <p> {ReasonReact.string("Here we go: " ++ view ++ id ++ network)} </p>
  </div>;