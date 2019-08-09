let getBlockTotalsThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Utils.getQueryForBlockTotals(id);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(conseilServerInfo, platform, network, "operations", query)
    |> then_(totals =>
        if (totals |> Js.Array.length > 0) {
          resolve(Some(totals[0]));
        } else {
          resolve(None);
        }
      )
    |> catch(_err => resolve(None))
  );
};

let getBlockHeadThunk = (config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlockHead(conseilServerInfo, network)
    |> then_(head => resolve(Some(head)))
    |> catch(_err => resolve(None))
  );
};

let getBlockThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((ConseiljsRe.TezosConseilClient.getBlock(conseilServerInfo, network, id), getBlockTotalsThunk(id, config)))
    |> then_(((blocks, totals)) => {
      let blockLength = Js.Array.length(blocks);
      switch (blockLength, totals) {
        | (1, Some(total)) => {
          let realBlock = Utils.convertBlock(~block=blocks[0], ~total=total, ());
          resolve(("Valid", None, Some(realBlock)));
        }
        | (1, None) => {
          let realBlock = Utils.convertBlock(~block=blocks[0], ());
          resolve(("Valid", None, Some(realBlock)));
        }
        | _ => resolve(("Error", Some(Utils.invalidId), None))
      };
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvaialbel), None)))
  );
};

let getBlockHashThunk = (level: int, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlockByLevel(conseilServerInfo, network, level)
    |> then_(hashs => {
      if(hashs |> Js.Array.length > 0) {
        resolve(Some(hashs[0]));
      } else {
        resolve(None);
      }
    })
    |> catch(_err => resolve(None))
  );
};

let getAccountBakerThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Utils.getQueryForBakerInfo(id);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(conseilServerInfo, platform, network, "delegates", query)
    |> then_(delegates =>
        if (delegates |> Js.Array.length > 0) {
          resolve(Some(delegates[0]));
        } else {
          resolve(None);
        }
      )
    |> catch(_err => resolve(None))
  );
};

let getOperationThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  let query = Utils.getQueryForOperations(id);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getOperations(conseilServerInfo, network, query)
    |> then_(operations => {
        if(operations |> Js.Array.length > 0) {
          let realOp = operations |> Array.map(op => Utils.convertOperation(op));
          resolve(("Valid", None, Some(realOp)));
        } else {
          resolve(("Error", Some(Utils.invalidId), None))
        }
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvaialbel), None)))
  );
};

let getAccountThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((ConseiljsRe.TezosConseilClient.getAccount(conseilServerInfo, network, id), getAccountBakerThunk(id, config)))
    |> then_(((accounts, bakers)) => {
      let accountLength = Js.Array.length(accounts);
      switch (accountLength, bakers) {
        | (1, Some(baker)) => {
          let realAcc = Utils.convertAccount(~account=accounts[0], ~baker=baker, ());
          resolve(("Valid", None, Some(realAcc)));
        }
        | (1, None) => {
          let realAcc = Utils.convertAccount(~account=accounts[0], ());
          resolve(("Valid", None, Some(realAcc)));
        }
        | _ => resolve(("Error", Some(Utils.invalidId), None))
      };
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvaialbel), None)))
  );
};