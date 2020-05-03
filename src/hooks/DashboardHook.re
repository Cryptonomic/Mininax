open GlobalStore;
open DashboardStore;
open Configs;
let selector = (state: GlobalStore.globalState) =>
  state.appState.selectedConfig;
module Make = (()) => {
  let dispatch = Store.useDispatch();
  let selectedConfig = Store.useSelector(selector);

  let getProposalsInfo = (metaCycle: int) =>
    ApiCall.getProposalInfoThunk(
      ~metaCycle,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(proposals) =>
          dispatch(DashboardAction(SetProposals(proposals)))
        | _ => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
    );

  let getVoteInfo = (hash: string, active_proposal: string) =>
    ApiCall.getVoteInfoThunk(
      ~hash,
      ~active_proposal,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some(voteinfo) =>
          dispatch(DashboardAction(SetVoteInfo(voteinfo)))
        | _ => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
    );

  let getBlockInfo = (block: ConseiljsType.tezosBlock) =>
    ApiCall.getBlockInfoThunk(
      ~metaCycle=block##meta_cycle,
      ~timestamp=block##timestamp,
      ~config=configs[selectedConfig],
      ~callback=
        fun
        | Some((blockinfo, transinfo)) => {
            Js.log(block);
            Js.log(block |> Decode.json_of_magic |> Decode.parseLatestBlock);
            dispatch(
              DashboardAction(SetLastBlock(block, blockinfo, transinfo)),
              // DashboardAction(Set)
            );
            dispatch(AppAction(SetLoaded));
          }
        | _ => dispatch(AppAction(SetError(ErrMessage.noAvailable))),
    );
};

type otherTotals =
  | CountedTransactions24h(int)
  | CountedZeroPriorityBlocksLevels24h(int)
  | CountedBakers24h(int)
  | CountOriginationAndReveal(option(int), option(int))
  | GetTop3Bakers(array(baker))
  | GetStorageDelta24h(storageDelta);