open GlobalStore;
open DashboardStore;
open Configs;
open DashboardApi.Thunk;

let selector = (state: GlobalStore.globalState) =>
  state.appState.selectedConfig;
module Make = (()) => {
  let dispatch = Store.useDispatch();
  let selectedConfig = Store.useSelector(selector);

  let getProposalsInfo = (block: ConseiljsType.tezosBlock) =>
    getProposalsInfoThunk(
      ~config=configs[selectedConfig],
      ~metaCycle=block##meta_cycle,
    )
    ->Future.get(value =>
        dispatch(DashboardAction(SetProposalInfo(value)))
      );

  let getGovernanceProcessInfo = (block: ConseiljsType.tezosBlock) =>
    getGovernanceProcessInfoThunk(
      ~config=configs[selectedConfig],
      ~hash=block##hash,
      ~active_proposal=block##active_proposal,
    )
    ->Future.get(value =>
        dispatch(DashboardAction(SetGovernanceProcess(value)))
      );

  // Replace to combined
  let getTotalsInfo = () =>
    getTotalsInfoThunk(
      ~timestamp=Js.Date.make() |> Js.Date.getTime,
      ~config=configs[selectedConfig],
    )
    ->Future.map(value => [DashboardAction(SetTotalsInfo(value))]);

  let getBakersInfo = () =>
    getBakersInfoThunk(~config=configs[selectedConfig])
    ->Future.map(value => [DashboardAction(SetBackerInfo(value))]);

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    getBlockInfoThunk(
      ~metaCycle=block##meta_cycle,
      ~timestamp=Js.Date.make() |> Js.Date.getTime,
      ~config=configs[selectedConfig],
    )
    ->Future.map(value => {
        [
          DashboardAction(
            SetLastBlock(
              block |> Decode.json_of_magic |> Decode.parseLatestBlock,
            ),
          ),
          DashboardAction(SetBlockInfo(value)),
        ]
      });

  let getTheLatestGovernanceInfo = () => {
    DashboardApi.Calls.getTheLatestGovernance(~config=configs[selectedConfig])
    ->Future.map(value => [DashboardAction(SetLatestGovernance(value))]);
  };

  let getDashboardInfo = (block: ConseiljsType.tezosBlock) =>
    Future.all([
      getBlockInfo(block),
      getTotalsInfo(),
      getBakersInfo(),
      getTheLatestGovernanceInfo(),
    ])
    ->Future.map(List.flatten)
    ->Future.map(List.map(dispatch));
};