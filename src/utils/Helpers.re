let applyQuery = (f, ~query) => f(query);
let apply1 = (f, param) => f(param);
let applyField = (f, ~field) => f(field);
let applyTuple3 = (f, ~tuple) => {
  let (x, y, z) = tuple;
  f(x, y, z);
};
let applyTuple3SkipSecond = (f, ~tuple) => {
  let (x, _, z) = tuple;
  f(x, z);
};
let toOption = value => Some(value);
let toResult = value => Ok(value);

let slice = (value: string, ~to_: int) =>
  value |> Js.String.slice(~from=0, ~to_) |> Js.String.toLowerCase;

