open GlobalStore;

ReactDOMRe.renderToElementWithId(
  <Store.Provider store=globalStore> <App /> </Store.Provider>,
  "root",
);