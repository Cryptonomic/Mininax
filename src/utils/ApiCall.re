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

let getBlockFromApi = (conseilServerInfo: Type.conseilServerInfo, network: string, id: string) => {  Js.Promise.(
  ConseiljsRe.TezosConseilClient.getBlock(conseilServerInfo, network, id)
    |> then_(block => resolve(Some(block)))
    |> catch(_err => resolve(None))
  );
};

let getBlockThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((getBlockFromApi(conseilServerInfo, network, id), getBlockTotalsThunk(id, config)))
    |> then_(((blocks, totals)) => {
      switch (blocks, totals) {
        | (Some(block), Some(total)) => {
          let realBlock = Utils.convertBlock(~block=block, ~total=total, ());
          resolve(("Valid", None, Some(realBlock)));
        }
        | (Some(block), None) => {
          let realBlock = Utils.convertBlock(~block=block, ());
          resolve(("Valid", None, Some(realBlock)));
        }
        | _ => resolve(("Error", Some(Utils.invalidId), None))
      };
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None)))
  );
};

let getBlockHashThunk = (level: int, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlockByLevel(conseilServerInfo, network, level)
    |> then_(hashes => {
      if (hashes |> Js.Array.length > 0) {
        resolve(Some(hashes[0]));
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
            resolve(("Error", Some(Utils.operationNotFound), None))
        }
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None)))
  );
};

let getAccountFromApi = (conseilServerInfo: Type.conseilServerInfo, network: string, id: string) => { 
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getAccount(conseilServerInfo, network, id)
      |> then_(account => resolve(Some(account)))
      |> catch(_err => resolve(None))
  );
};

let getAccountThunk = (id: string, config: Type.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((getAccountFromApi(conseilServerInfo, network, id), getAccountBakerThunk(id, config)))
    |> then_(((accounts, bakers)) => {
      switch (accounts, bakers) {
        | (Some(account), Some(baker)) => {
          let realAcc = Utils.convertAccount(~account=account, ~baker=baker, ());
          resolve(("Valid", None, Some(realAcc)));
        }
        | (Some(account), None) => {
          let realAcc = Utils.convertAccount(~account=account, ());
          resolve(("Valid", None, Some(realAcc)));
        }
        | _ => {
            let twoChars = id |> Js.String.slice(~from=0, ~to_=2) |> Js.String.toLowerCase;
            if (twoChars === "tz") {
                resolve(("Error", Some(Utils.missingAccount), None));
            } else if (twoChars === "kt") {
                resolve(("Error", Some(Utils.missingContract), None));
            } else {
                resolve(("Error", Some(Utils.invalidAccountId), None));
            }
        }
      };
    })
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None)))
  );
};
