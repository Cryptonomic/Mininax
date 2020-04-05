// TODO wrap all functions with Future
// TODO refactor all function to pipe

/*
    In general ReasonML community complain a lot about Js.Promise module and how it handle asynchronus code.
    It has "resolve" part you hadn't used here, it doesn't well play with type system, and it's hard to abort Promise (thought, you still can with AbortController from JS api).
    What alternative we have?
    reason-future, reason-promise, bs-rx
    We're going to wrap promise with Future, and modify code to make it easier to use in components. Future gives us functionality to process data in pipes. Checkout docs: https://github.com/rationaljs/future

    Another important thing here is how result from Promise/Future is handled.
    In some functions you used return type to be option('a) and it's absolute fine,
    but in others result type is a tuple (for exmaple for getBlockThunk, getOperationThunk, etc) with pattern
    (string as result type, error value wrapped with option, success value wrapped with option)
    downsides:
    1. string as result type is not a great idea. We can't use full power of reasonML type system with it.
    2. you have to wrap error and success result with option

    There is pretty elegant way to handle that kind of situation with type system and variants:
    type result('a,'b) =
      | OK('a)
      | Error('b);
    It's mentioned in Reason documentation: https://reasonml.github.io/docs/en/more-on-type
    but I think the nicer description you can find in Rust documentation (another language, but idea stays the same): https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html

    So, in this case you don't need tuple with 3 values, you can return Ok(tuple) for success and Error(value) for failure, and you can handle it the same way with pattern matching and option.


   Opening modules (`JS.Promise.(...)`) is a great way to work, but I'm not going to do that here to make example more descriptive. You can still use it in your code.
   We're going to think about how to exchage this function into function that pipes data
   Why?
   It will make cleaner code to process all data in functions and just execute callback provided from react component (it might be for example "set" function from useState, callback for dispatch, etc.)
   1. We want to free react component from logic so we'd like to define all logic here
   2. We're going to wrap promises with reason-future for easier data processing

   We need to define some helper function in order to pipe all data
   1. we need to get values for  `ConseiljsRe.ConseilDataClient.executeEntityQuery`
   executeEntityQuery need 2 ather functions to be executed in order to get all value it requires
   In normal situation for applying 2 functions result to third function we would use function like Ramda.js' converge. Problem we have here is Utils.getInfo return tuple, so we need a helper funciton,
   to apply tuple elements as invidual parameters

   You can refactor rest of the code the same way.
 */

let applyTuple3 = (f, ~tuple) => {
  let (x, y, z) = tuple;
  f(x, y, z);
};

let applyQuery = (f, ~query) => f(query);

let applyTuple3SkipSecond = (f, ~tuple) => {
  let (x, _, z) = tuple;
  f(x, z);
};

let labelExecuteEntityQuery = (info, platform, network, ~field, query) =>
  ConseiljsRe.ConseilDataClient.executeEntityQuery(
    info,
    platform,
    network,
    field,
    query,
  );

let labelGetBlock = (conseilServerInfo, network, ~id) =>
  ConseiljsRe.TezosConseilClient.getBlock(conseilServerInfo, network, id);

// we need to return Future here, because it is used in Promise.all / Future.all later
let getBlockTotalsThunk = (~id: string, ~config: MainType.config) =>
  // we start applying parameters here, because reason automatically curry function without all parameters we can do that in steps
  labelExecuteEntityQuery(~field="operations")
  ->applyTuple3(~tuple=Utils.getInfo(config))
  ->applyQuery(~query=Queries.getQueryForBlockTotals(id))
  // we applied all parameters and got promise
  // we're going to exchange promise to future, callback is required for handling `catch`. Check out future docs
  ->FutureJs.fromPromise(_err => None)
  //flat map allows us to get value from future and transform it into another future
  ->Future.flatMap(value => {
      switch (value) {
      // we use 'when' clause to not repeat callback(None)
      | Ok(totals) when totals |> Js.Array.length > 0 =>
        // returns future
        Future.value(Some(totals[0]))
      | _err => Future.value(None)
      }
    });

// it is used in react component, it can return unit type and execute callback
let getBlockHeadThunk = (~callback, ~config: MainType.config) =>
  ConseiljsRe.TezosConseilClient.getBlockHead
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->FutureJs.fromPromise(_err => None)
  // map takes value from Future and return other value that is not Future
  ->Future.map(
      fun
      | Ok(result) => Some(result)
      | _err => None,
    )
  ->Future.get(callback);

let getBlockFromApi = (~id: string, ~config: MainType.config) =>
  labelGetBlock(~id)
  ->applyTuple3SkipSecond(~tuple=Utils.getInfo(config))
  ->FutureJs.fromPromise(_err => None)
  ->Future.flatMap(value =>
      switch (value) {
      | Ok(block) => Future.value(Some(block))
      | _err => Future.value(None)
      }
    );

let getBlockThunk = (~callback, ~id: string, ~config: MainType.config) =>
  // return types from getBlockFromApi & getBlockTotalsThunk are different, Future.all expects list('a), so it won't fit,
  // instead we are going to use Js.Promise.all2 as before and convert params to promise and than result to future
  Js.Promise.all2((
    FutureJs.toPromise(getBlockFromApi(~id, ~config)),
    FutureJs.toPromise(getBlockTotalsThunk(~id, ~config)),
  ))
  ->FutureJs.fromPromise(_err => None)
  ->Future.map(result => {
      switch (result) {
      // Destruction can be done in switch
      // Also as you can see, using build in result type make code much cleaner
      // we got Ok(option('a), option('b)) because we set this 2 promises to result option('a)
      | Ok((Some(block), Some(total))) =>
        let realBlock = Utils.convertBlock(~block, ~total, ());
        // but we don't have to wrap option with result anymore, we can just pass result with tuple
        Ok((realBlock, block));
      | Ok((Some(block), None)) =>
        let realBlock = Utils.convertBlock(~block, ());
        Ok((realBlock, block));
      | Ok((None, None)) => Error(Utils.noAvailable)
      | _err => Error(Utils.invalidId)
      }
    })
  // Future.get gets value from promise and pass it to callback
  ->Future.get(callback);

let getBlockHashThunk = (level: int, config: MainType.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlockByLevel(
      conseilServerInfo,
      network,
      level,
    )
    |> then_(hashes =>
         if (hashes |> Js.Array.length > 0) {
           resolve(Some(hashes[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getAccountBakerThunk = (id: string, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForBakerInfo(id);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "delegates",
      query,
    )
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

let getOperationThunk = (id: string, config: MainType.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  let query = Queries.getQueryForOperations(id);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getOperations(
      conseilServerInfo,
      network,
      query,
    )
    |> then_(operations =>
         if (operations |> Js.Array.length > 0) {
           let realOp =
             operations |> Array.map(op => Utils.convertOperation(op));
           resolve(("Valid", None, Some(realOp)));
         } else {
           resolve(("Error", Some(Utils.operationNotFound), None));
         }
       )
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None)))
  );
};

let getAccountFromApi =
    (
      conseilServerInfo: MainType.conseilServerInfo,
      network: string,
      id: string,
    ) => {
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getAccount(conseilServerInfo, network, id)
    |> then_(account => resolve(Some(account)))
    |> catch(_err => resolve(None))
  );
};

let getAccountThunk = (id: string, config: MainType.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((
      getAccountFromApi(conseilServerInfo, network, id),
      getAccountBakerThunk(id, config),
    ))
    |> then_(((accounts, bakers)) => {
         switch (accounts, bakers) {
         | (Some(account), Some(baker)) =>
           let realAcc = Utils.convertAccount(~account, ~baker, ());
           resolve(("Valid", None, Some(realAcc)));
         | (Some(account), None) =>
           let realAcc = Utils.convertAccount(~account, ());
           resolve(("Valid", None, Some(realAcc)));
         | _ =>
           let twoChars =
             id |> Js.String.slice(~from=0, ~to_=2) |> Js.String.toLowerCase;
           if (twoChars === "tz") {
             resolve(("Error", Some(Utils.missingAccount), None));
           } else if (twoChars === "kt") {
             resolve(("Error", Some(Utils.missingContract), None));
           } else {
             resolve(("Error", Some(Utils.invalidAccountId), None));
           };
         }
       })
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None)))
  );
};

let getNumBlocksApi =
    (metaCycle: int, timeStamp: float, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForNumBlocks(metaCycle, timeStamp);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "blocks",
      query,
    )
    |> then_(numBlocks =>
         if (numBlocks |> Js.Array.length > 0) {
           resolve(Some(numBlocks[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getTransactionsApi = (timeStamp: float, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForTransactionStats(timeStamp);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
    |> then_(transactions =>
         if (transactions |> Js.Array.length > 0) {
           resolve(Some(transactions[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getFundraiserApi = (timeStamp: float, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForFundraiserStats(timeStamp);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
    |> then_(fundraisers =>
         if (fundraisers |> Js.Array.length > 0) {
           resolve(Some(fundraisers[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getTotalFundraiserApi = (config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForTotalFundraiserActivated();
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
    |> then_(fundraisers =>
         if (fundraisers |> Js.Array.length > 0) {
           resolve(Some(fundraisers[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getFeesStatsApi = (timeStamp: float, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForFeesStats(timeStamp);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
    |> then_(fees =>
         if (fees |> Js.Array.length > 0) {
           resolve(Some(fees[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getBakersStatsApi = (config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForBakerStats();
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "delegates",
      query,
    )
    |> then_(bakers =>
         if (bakers |> Js.Array.length > 0) {
           resolve(Some(bakers[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getMarketCapApi = (config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForMarketCap();
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "accounts",
      query,
    )
    |> then_(totalTez =>
         if (totalTez |> Js.Array.length > 0) {
           resolve(Some(totalTez[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getQuorumInfoApi = (hash: string, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForQuorum(hash);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "blocks",
      query,
    )
    |> then_(quorumStats =>
         if (quorumStats |> Js.Array.length > 0) {
           resolve(Some(quorumStats[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getVotingStatsApi =
    (hash: string, proposal: string, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForVotingStats(hash, proposal);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "governance",
      query,
    )
    |> then_(votingStats =>
         if (votingStats |> Js.Array.length > 0) {
           resolve(Some(votingStats[0]));
         } else {
           resolve(None);
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getVoteInfoThunk =
    (hash: string, active_proposal: string, config: MainType.config) => {
  Js.Promise.(
    all2((
      getQuorumInfoApi(hash, config),
      getVotingStatsApi(hash, active_proposal, config),
    ))
    |> then_(result => {
         switch (result) {
         | (Some(quorumStat), Some(votingStat)) =>
           let quorumObj = quorumStat |> Obj.magic;
           let votingObj = votingStat |> Obj.magic;
           let votinfo: MainType.voteInfo = {
             yay_rolls: votingObj##yay_rolls,
             nay_rolls: votingObj##nay_rolls,
             pass_rolls: votingObj##pass_rolls,
             proposal_hash: votingObj##proposal_hash,
             current_expected_quorum: quorumObj##current_expected_quorum,
           };
           resolve(Some(votinfo));
         | _ => resolve(None)
         }
       })
    |> catch(_err => resolve(None))
  );
};

let getProposalInfoThunk = (metaCycle: int, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForProposalInfo(metaCycle);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
    |> then_(proposalStats =>
         if (proposalStats |> Js.Array.length > 0) {
           let newProposals =
             proposalStats
             |> Array.map(proposal => {
                  let proposalObj = proposal |> Obj.magic;
                  let newProposal: MainType.proposalInfo = {
                    count_operation_group_hash:
                      proposalObj##count_operation_group_hash,
                    proposal: proposalObj##proposal,
                  };
                  newProposal;
                });
           resolve(Some(newProposals));
         } else {
           resolve(Some([||]));
         }
       )
    |> catch(_err => resolve(None))
  );
};

let getBlockInfoThunk =
    (metaCycle: int, timeStamp: float, config: MainType.config) => {
  let apis = [|
    getNumBlocksApi(metaCycle, timeStamp, config),
    getTransactionsApi(timeStamp, config),
    getFundraiserApi(timeStamp, config),
    getTotalFundraiserApi(config),
    getFeesStatsApi(timeStamp, config),
    getBakersStatsApi(config),
    getMarketCapApi(config),
  |];
  Js.Promise.(
    all(apis)
    |> then_(result =>
         if (result |> Js.Array.length > 0) {
           let newTransInfoObj = result[1] |> Obj.magic;
           let newTranInfo: MainType.transInfo = {
             countOriginatedContracts:
               newTransInfoObj##count_originated_contracts,
             countAmount: newTransInfoObj##count_amount,
             sumAmount: newTransInfoObj##sum_amount,
           };
           let newFundraiser = result[2] |> Obj.magic;
           let newTotalFundraiser = result[3] |> Obj.magic;
           let feesObj = result[4] |> Obj.magic;
           let bakersObj = result[5] |> Obj.magic;
           let marketObj = result[6] |> Obj.magic;

           let newBlockInfo: MainType.blockInfo = {
             blockCount: (result[0] |> Obj.magic)##count_hash,
             fundraiserCount: newFundraiser##count_kind,
             totalFundraiserCount: newTotalFundraiser##count_kind,
             sum_fee: feesObj##sum_fee,
             sum_consumed_gas: feesObj##sum_consumed_gas,
             num_bakers: bakersObj##count_pkh,
             bakers_sum_staking_balance: bakersObj##sum_staking_balance,
             totalTez: marketObj##sum_balance,
           };

           resolve((Some(newBlockInfo), Some(newTranInfo)));
         } else {
           resolve((None, None));
         }
       )
    |> catch(_err => resolve((None, None)))
  );
};