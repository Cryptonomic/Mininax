open Styles1;
[@react.component]
let make = (~items, ~goToDetail) => {
  let theme = React.useContext(ContextProvider.themeContext);
  let (count, setCount) = React.useState(() => 0);
  let total = items |> Js.Array.length;
  let changeLevel = (level) => {
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
            <div className=transactionBtn(theme, count === 0) onClick={_=>setCount(count => count - 1)}>{ReasonReact.string("<")}</div>
            <div className=transactionBtn(theme, count === total - 1) onClick={_=>setCount(count => count + 1)}>{ReasonReact.string(">")}</div>
          </div>
        </div>
      ) :
      (
        <div className=title(theme)>{ReasonReact.string("Operation")}</div>
      )
    }
    <ItemDisplay entity="operation" items=items[count] changeLevel={changeLevel} goToDetail={goToDetail} />
  </div>
}
