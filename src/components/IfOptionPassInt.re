type props = int => React.element;
let make = (~children: props, ~validator) => {
  switch (validator) {
  | Some(value) => children(value)
  | None => React.null
  };
};