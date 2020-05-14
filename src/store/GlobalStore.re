type allStoreActions =
  | AppAction(AppStore.action)
  | DashboardAction(DashboardStore.action);

type globalState = {
  appState: AppStore.state,
  dashboardState: DashboardStore.state,
};

let globalReducer = (state, action: allStoreActions) => {
  switch (action) {
  | AppAction(x) => {...state, appState: AppStore.reducer(state.appState, x)}
  | DashboardAction(x) => {
      ...state,
      dashboardState: DashboardStore.reducer(state.dashboardState, x),
    }
  };
};

let globalStore =
  Reductive.Store.create(
    ~reducer=globalReducer,
    ~preloadedState={
      appState: AppStore.initState,
      dashboardState: DashboardStore.initState,
    },
    (),
  );

module Store = {
  include ReductiveContext.Make({
    type action = allStoreActions;
    type state = globalState;
  });
};