open Configs;
open GlobalStore;
open AppStore;

let selector = (state: GlobalStore.globalState) =>
  state.appState.selectedConfig;

module Make = (()) => {
  let dispatch = Store.useDispatch();
  let selectedConfig = Store.useSelector(selector);
  module UseDashboard =
    DashboardHook.Make({});

  let getBlock = (id: string, isRoute: bool, isMain: bool, level: int) => {
    dispatch(AppAction(SetLoading));
    let callback = result =>
      switch (result) {
      | Ok((block, lastBlock)) =>
        switch (isRoute, isMain, level) {
        | (true, false, 0) =>
          let url =
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="blocks",
              ~id,
            );
          ReasonReactRouter.push(url);
          dispatch(AppAction(SetBlock(block, id, false)));
        | (true, false, _) =>
          let strLevel = string_of_int(level);
          let url =
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="blocks",
              ~id=strLevel,
            );
          ReasonReactRouter.push(url);
          dispatch(AppAction(SetBlock(block, strLevel, false)));
        | (false, false, _) =>
          dispatch(AppAction(SetBlock(block, id, false)))
        | _ =>
          dispatch(AppAction(SetBlock(block, "", true)));
          switch (lastBlock##period_kind) {
          | "proposal" => UseDashboard.getProposalsInfo(lastBlock)
          | "testing" => ()
          // TODO have to test
          | _ => UseDashboard.getGovernanceProcessInfo(lastBlock)
          };
          // TODO should run them together for set loaded in the same time
          UseDashboard.getTotalsInfo();
          UseDashboard.getBakersInfo();
          UseDashboard.getTheLatestGovernanceInfo();
          UseDashboard.getBlockInfo(lastBlock);
          ();
        }
      | Error(err) => dispatch(AppAction(SetError(err)))
      };
    ApiCall.getBlockThunk(~callback, ~id, ~config=configs[selectedConfig]);
  };

  let getOperation = (id: string, isRoute: bool) => {
    dispatch(AppAction(SetLoading));
    ApiCall.getOperationThunk(
      ~id,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Ok(operations) when isRoute == true => {
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="operations",
              ~id,
            )
            |> ReasonReactRouter.push;
            dispatch(AppAction(SetOperations(operations, id)));
          }
        | Ok(operations) =>
          dispatch(AppAction(SetOperations(operations, id)))
        | Error(err) => dispatch(AppAction(SetError(err))),
    );
  };

  let getAccount = (id: string, isRoute: bool) => {
    dispatch(AppAction(SetLoading));
    ApiCall.getAccountThunk(
      ~id,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Ok(account) when isRoute => {
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="accounts",
              ~id,
            )
            |> ReasonReactRouter.push;
            dispatch(AppAction(SetAccount(account, id)));
          }
        | Ok(account) => dispatch(AppAction(SetAccount(account, id)))
        | Error(err) => dispatch(AppAction(SetError(err))),
    );
  };

  let getHashByLevel = (level: int, ~isMain: bool) => {
    dispatch(AppAction(SetLoading));
    ApiCall.getBlockHashThunk(
      ~level,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(head) when isMain => getBlock(head##hash, false, true, level)
        | Some(head) => getBlock(head##hash, true, false, level)
        | _err => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
    );
  };

  let getMainPage = () => {
    dispatch(AppAction(SetLoading));
    let callback = result =>
      switch (result) {
      | Some(head) => getBlock(head##hash, false, true, 0)
      | None => dispatch(AppAction(SetError(ErrMessage.noAvailable)))
      };
    ApiCall.getBlockHeadThunk(~callback, ~config=configs[selectedConfig]);
    ();
  };

  let goToMainPage = network => {
      getMainPage();
    Js.log("Go to main page");
    ReasonReactRouter.push("/" ++ network);
  };

  let goToNetwork = network => {
    let selectedIndex =
      configs
      |> Js.Array.findIndex((conf: MainType.config) =>
           conf.network === network
         );
    switch (selectedIndex) {
    | (-1) => goToMainPage(network)
    | _ =>
      dispatch(AppAction(ChangeNetwork(selectedIndex)));
      //   setSelectedConfig(_old => selectedIndex);
      getMainPage();
    };
  };

  let goToPage = (network: string, entity: string, id: string) => {
    let selectedIndex =
      configs
      |> Js.Array.findIndex((conf: MainType.config) =>
           conf.network === network
         );
    let isNumber = id |> Utils.isNumber;
    switch (selectedIndex) {
    | (-1) => goToMainPage(network)
    | index when index != selectedConfig =>
      dispatch(AppAction(ChangeNetwork(selectedIndex)))
    | _ =>
      dispatch(AppAction(ChangeNetwork(selectedIndex)));
      //   setSelectedConfig(_old => selectedIndex);
      switch (entity, isNumber) {
      | ("blocks", false) => getBlock(id, false, false, 0)
      | ("blocks", true) =>
        getHashByLevel(id |> int_of_string, ~isMain=false)
      | ("accounts", false) => getAccount(id, false)
      | ("operations", false) => getOperation(id, false)
      | _ => goToMainPage(network)
      };
    };
  };

  let setFocusOfSearch = footerRef => {
    switch (footerRef) {
    | Some(el) =>
      let _ =
        Js.Global.setTimeout(
          _ => {
            let elementObj = ReactDOMRe.domElementToObj(el);
            ignore(elementObj##focus());
            elementObj##select();
          },
          100,
        );
      ();
    | None => ignore()
    };
  };

  let onChangeId = id => {
    let newId = id |> Js.String.replaceByRe([%re "/ /g"], "");
    dispatch(AppAction(SetId(newId)));
  };

  let onSearchById = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let twoChars =
      id |> Js.String.slice(~from=0, ~to_=2) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, twoChars, isNumber) {
    | ("b", _, _) => getBlock(id, true, false, 0)
    | ("o", _, _) => getOperation(id, true)
    | (_, "tz", _)
    | (_, "kt", _) => getAccount(id, true)
    | (_, _, true) => getHashByLevel(id |> int_of_string, ~isMain=false)
    | _ => dispatch(AppAction(SetError(ErrMessage.invalidId)))
    };
  };

  let onSearchMain = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, isNumber) {
    | ("b", _) => getBlock(id, true, true, 0)
    | (_, true) => getHashByLevel(id |> int_of_string, ~isMain=true)
    | _ => dispatch(AppAction(SetError(ErrMessage.invalidId)))
    };
  };

  let onChangeNetwork = (index: int) =>
    if (selectedConfig !== index) {
      dispatch(AppAction(ChangeNetwork(index)));
      //   setSelectedConfig(_old => index);
      goToMainPage(configs[index].network);
    } else {
      dispatch(AppAction(OpenNetwork(false)));
    };
};