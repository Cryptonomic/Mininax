open Configs;
open GlobalStore;

let selector = (state: MainType.state) => state.selectedConfig;

module Make = (()) => {
  let dispatch = AppStore.useDispatch();
  let selectedConfig = AppStore.useSelector(selector);
  let getProposalsInfo = (metaCycle: int) =>
    ApiCall.getProposalInfoThunk(
      ~metaCycle,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(proposals) => dispatch(SetProposals(proposals))
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getVoteInfo = (hash: string, active_proposal: string) =>
    ApiCall.getVoteInfoThunk(
      ~hash,
      ~active_proposal,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(voteinfo) => dispatch(SetVoteInfo(voteinfo))
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    ApiCall.getBlockInfoThunk(
      ~metaCycle=block##meta_cycle,
      ~timestamp=block##timestamp,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some((blockinfo, transinfo)) => {
            dispatch(SetLastBlock(block, blockinfo, transinfo));
          }
        | _ => dispatch(SetError(ErrMessage.noAvailable)),
    );

  let getBlock = (id: string, isRoute: bool, isMain: bool, level: int) => {
    dispatch(SetLoading);
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
          dispatch(SetBlock(block, id, false));
        | (true, false, _) =>
          let strLevel = string_of_int(level);
          let url =
            Utils.makeUrl(
              ~network=configs[selectedConfig].network,
              ~entity="blocks",
              ~id=strLevel,
            );
          ReasonReactRouter.push(url);
          dispatch(SetBlock(block, strLevel, false));
        | (false, false, _) => dispatch(SetBlock(block, id, false))
        | _ =>
          dispatch(SetBlock(block, "", true));
          switch (lastBlock##period_kind) {
          | "proposal" => getProposalsInfo(lastBlock##meta_cycle)
          | "testing" => ()
          | _ => getVoteInfo(lastBlock##hash, lastBlock##active_proposal)
          };
          getBlockInfo(lastBlock);
        }
      | Error(err) => dispatch(SetError(err))
      };
    ApiCall.getBlockThunk(~callback, ~id, ~config=configs[selectedConfig]);
  };

  let getOperation = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
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
            dispatch(SetOperations(operations, id));
          }
        | Ok(operations) => dispatch(SetOperations(operations, id))
        | Error(err) => dispatch(SetError(err)),
    );
  };

  let getAccount = (id: string, isRoute: bool) => {
    dispatch(SetLoading);
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
            dispatch(SetAccount(account, id));
          }
        | Ok(account) => dispatch(SetAccount(account, id))
        | Error(err) => dispatch(SetError(err)),
    );
  };

  let getHashByLevel = (level: int, ~isMain: bool) => {
    dispatch(SetLoading);
    ApiCall.getBlockHashThunk(
      ~level,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(head) when isMain => getBlock(head##hash, false, true, level)
        | Some(head) => getBlock(head##hash, true, false, level)
        | _err => dispatch(SetError(ErrMessage.noAvailable)),
    );
  };

  let getMainPage = () => {
    dispatch(SetLoading);
    let callback = result =>
      switch (result) {
      | Some(head) => getBlock(head##hash, false, true, 0)
      | None => dispatch(SetError(ErrMessage.noAvailable))
      };
    ApiCall.getBlockHeadThunk(~callback, ~config=configs[selectedConfig]);
    ();
  };

  let goToMainPage = () => {
    getMainPage();
    ReasonReactRouter.push("/" ++ configs[selectedConfig].network);
  };

  let goToNetwork = network => {
    let selectedIndex =
      configs
      |> Js.Array.findIndex((conf: MainType.config) =>
           conf.network === network
         );
    switch (selectedIndex) {
    | (-1) => goToMainPage()
    | _ =>
      dispatch(ChangeNetwork(selectedIndex));
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
    | (-1) => goToMainPage()
    | _ =>
      dispatch(ChangeNetwork(selectedIndex));
      //   setSelectedConfig(_old => selectedIndex);
      switch (entity, isNumber) {
      | ("blocks", false) => getBlock(id, false, false, 0)
      | ("blocks", true) =>
        getHashByLevel(id |> int_of_string, ~isMain=false)
      | ("accounts", false) => getAccount(id, false)
      | ("operations", false) => getOperation(id, false)
      | _ => goToMainPage()
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
    dispatch(SetId(newId));
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
    | _ => dispatch(SetError(ErrMessage.invalidId))
    };
  };

  let onSearchMain = (id: string) => {
    let firstChar =
      id |> Js.String.slice(~from=0, ~to_=1) |> Js.String.toLowerCase;
    let isNumber = id |> Utils.isNumber;
    switch (firstChar, isNumber) {
    | ("b", _) => getBlock(id, true, true, 0)
    | (_, true) => getHashByLevel(id |> int_of_string, ~isMain=true)
    | _ => dispatch(SetError(ErrMessage.invalidId))
    };
  };

  let onChangeNetwork = (index: int) =>
    if (selectedConfig !== index) {
      dispatch(ChangeNetwork(index));
      //   setSelectedConfig(_old => index);
      goToMainPage();
    } else {
      dispatch(OpenNetwork(false));
    };
};