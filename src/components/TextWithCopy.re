[@react.component]
let make = (~value: string, ~className="", ~isReverse=false) =>
  <div className>
    {value |> ReasonReact.string}
    <CopyContent isReverse hash=value />
  </div>;