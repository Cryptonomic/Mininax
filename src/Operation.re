open Styles1;
[@react.component]
let make = (~items, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let (count, setCount) = React.useState(() => 0);
  let total = items |> Js.Array.length;
  let changeLevel = (level) => {
    Js.log2("level", level);
    ();
  };
  <div className=container(theme)>
    {total > 1 ? 
      (
        <div className=titleContainer>
          <div className=title(theme)>
            {ReasonReact.string("Operation (" ++ string_of_int(count+1) ++ "/" ++ string_of_int(total) ++ ")" )}
          </div>
          <div className=titleBtnContainer>
            <button disabled={count === 0} className=transactionBtn(theme, count === 0) onClick={_=>setCount(count => count - 1)}>{ReasonReact.string("<")}</button>
            <button disabled={count === total - 1} className=transactionBtn(theme, count === total - 1) onClick={_=>setCount(count => count + 1)}>{ReasonReact.string(">")}</button>
          </div>
        </div>
      ) :
      (
        <div className=title(theme)>{ReasonReact.string("Operation")}</div>
      )
    }
    {total > 0 ? <ItemDisplay entity="operation" items=items[count] changeLevel goToDetail /> : ReasonReact.null}
  </div>
}
