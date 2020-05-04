open GlobalStore;
open DashboardStore;
open Configs;
open DashboardApi.Thunk;

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

  let getProposalsInfo = (block: ConseiljsType.tezosBlock) =>
    getProposalsInfoThunk(
      ~config=configs[selectedConfig],
      ~metaCycle=block##meta_cycle,
      ~callback=value => {
      dispatch(DashboardAction(SetProposalInfo(value)))
    });

  let getGovernanceProcessInfo = (block: ConseiljsType.tezosBlock) =>
    getGovernanceProcessInfoThunk(
      ~config=configs[selectedConfig],
      ~hash=block##hash,
      ~active_proposal=block##active_proposal,
      ~callback=value => {
      dispatch(DashboardAction(SetGovernanceProcess(value)))
    });

  let getTotalsInfo = (block: ConseiljsType.tezosBlock) =>
    getTotalsInfoThunk(
      ~timestamp=block##timestamp,
      ~config=configs[selectedConfig],
      ~callback=value =>
      dispatch(DashboardAction(SetTotalsInfo(value)))
    );

  let getBakersInfo = () =>
    getBakersInfoThunk(~config=configs[selectedConfig], ~callback=value =>
      dispatch(DashboardAction(SetBackerInfo(value)))
    );

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    getBlockInfoThunk(
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