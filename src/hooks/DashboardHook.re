open GlobalStore;
open DashboardStore;
open Configs;
let selector = (state: GlobalStore.globalState) =>
  state.appState.selectedConfig;
module Make = (()) => {
  let dispatch = Store.useDispatch();
  let selectedConfig = Store.useSelector(selector);

  // let getProposalsInfo = (metaCycle: int) =>
  //   ApiCall.getProposalInfoThunk(
  //     ~metaCycle,
  //     ~config=configs[selectedConfig],
  //     ~callback=
  //       fun
  //       | Some(proposals) =>
  //         dispatch(DashboardAction(SetProposals(proposals)))
  //       | _ => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
  //   );

  // let getVoteInfo = (hash: string, active_proposal: string) =>
  //   ApiCall.getVoteInfoThunk(
  //     ~hash,
  //     ~active_proposal,
  //     ~config=configs[selectedConfig],
  //     ~callback=
  //       fun
  //       | Some(voteinfo) =>
  //         dispatch(DashboardAction(SetVoteInfo(voteinfo)))
  //       | _ => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
  //   );

  let getTotalsInfo = 

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    DashboardApi.getBlockInfoThunk(
      ~metaCycle=block##meta_cycle,
      ~timestamp=block##timestamp,
      ~config=configs[selectedConfig],
      ~callback=value => {
        dispatch(
          DashboardAction(
            SetLastBlock(
              block |> Decode.json_of_magic |> Decode.parseLatestBlock,
            ),
          ),
        );
        dispatch(DashboardAction(SetBlockInfo(value)));
        dispatch(AppAction(SetLoaded));
      },
    );
};