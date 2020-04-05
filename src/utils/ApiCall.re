// TODO wrapping all function with fn that carry MainType.config
// TODO wrap all functions with Future
// TODO refactor all function to pipe

let getBlockTotalsThunk = (id: string, config: MainType.config) => {
  let (conseilServerInfo, platform, network) = Utils.getInfo(config);
  let query = Queries.getQueryForBlockTotals(id);
  Js.Promise.(
    ConseiljsRe.ConseilDataClient.executeEntityQuery(
      conseilServerInfo,
      platform,
      network,
      "operations",
      query,
    )
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

let getBlockHeadThunk = (config: MainType.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlockHead(conseilServerInfo, network)
    |> then_(head => resolve(Some(head)))
    |> catch(_err => resolve(None))
  );
};

let getBlockFromApi =
    (
      conseilServerInfo: MainType.conseilServerInfo,
      network: string,
      id: string,
    ) => {
  Js.Promise.(
    ConseiljsRe.TezosConseilClient.getBlock(conseilServerInfo, network, id)
    |> then_(block => resolve(Some(block)))
    |> catch(_err => resolve(None))
  );
};

let getBlockThunk = (id: string, config: MainType.config) => {
  let (conseilServerInfo, _, network) = Utils.getInfo(config);
  Js.Promise.(
    all2((
      getBlockFromApi(conseilServerInfo, network, id),
      getBlockTotalsThunk(id, config),
    ))
    |> then_(((blocks, totals)) => {
         switch (blocks, totals) {
         | (Some(block), Some(total)) =>
           let realBlock = Utils.convertBlock(~block, ~total, ());
           resolve(("Valid", None, Some(realBlock), Some(block)));
         | (Some(block), None) =>
           let realBlock = Utils.convertBlock(~block, ());
           resolve(("Valid", None, Some(realBlock), Some(block)));
         | _ => resolve(("Error", Some(Utils.invalidId), None, None))
         }
       })
    |> catch(_err => resolve(("Error", Some(Utils.noAvailable), None, None)))
  );
};

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