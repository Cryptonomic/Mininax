open GlobalStore;

ReactDOMRe.renderToElementWithId(
  <AppStore.Provider store=globalStore> <App /> </AppStore.Provider>,
  "root",
);