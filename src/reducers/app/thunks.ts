import base64url from 'base64url';
import { TezosConseilClient, ConseilDataClient } from 'conseiljs';
import { setLoadingAction, setItemsAction, setErrorAction } from './actions';
import { Config } from '../../types';
import {
  getQueryForBlockTotals,
  convertFromUtezToTez,
  getQueryForOperations,
  getQueryForBlockLink,
  getQueryForBakerInfo,
  getQueryForAccountSends,
  getQueryForAccountReceipts,
  getQueryForOtherOperations,
  getQueryForEndorsements,
  getQueryForBakedBlocks,
  getQueryForDepositsAndRewards
} from '../../utils/general';

const InvalidId = 'You entered an invalid ID.';
const NoBlock = 'There is no block for the Level.';
const NoAvaialbe = 'Not available';
const arronaxURL = 'https://arronax-beta.cryptonomic.tech/';

function getInfo(config: Config) {
  const { platform, network, url, apiKey } = config;
  const conseilServer = { url, apiKey };
  return { conseilServer, platform, network };
}

export const getBlockThunk = (id: string) => async (dispatch, state) => {
  dispatch(setLoadingAction(true));
  const { selectedConfig } = state().app;
  const entity = 'block';
  const { conseilServer, network, platform } = getInfo(selectedConfig);
  const query = getQueryForBlockTotals(id);
  const block = await TezosConseilClient.getBlock(conseilServer, network, id).catch(e => {
    console.error('block', e);
    return [];
  });
  if (block.length > 0) {
    const totals = await ConseilDataClient.executeEntityQuery(conseilServer, platform, network, 'operations', query).catch(e => {
      return [];
    });

    if (totals.length > 0) {
      const item = {
        ...block[0],
        'consumed_gas': convertFromUtezToTez(block[0].consumed_gas),
        total_amount: convertFromUtezToTez(totals[0].sum_amount),
        fee: convertFromUtezToTez(totals[0].sum_fee),
        'baker_priority': 'Coming soon'
      };
      dispatch(setItemsAction(entity, item));
      return item;
    } else {
      dispatch(setErrorAction(NoAvaialbe, entity));
      return '';
    }
  } else {
    dispatch(setErrorAction(InvalidId, entity));
    return '';
  }
};

export const getAccountThunk = (id: string) => async (dispatch, state) => {
  dispatch(setLoadingAction(true));
  const { selectedConfig } = state().app;
  const entity = 'account';
  const { conseilServer, network, platform } = getInfo(selectedConfig);
  const account = await TezosConseilClient.getAccount(conseilServer, network, id).catch(e => {
    console.error('account', e);
    return [];
  });
  if (account.length > 0) {
    const query = getQueryForBakerInfo(id);
    const result = await ConseilDataClient.executeEntityQuery(conseilServer, platform, network, 'delegates', query).catch(e => {
      return [];
    });
    let item;
    if (result.length > 0) {
      item = {
        ...account[0],
        balance: convertFromUtezToTez(account[0].balance),
        'baker_deactivated': result[0].deactivated ? 'No' : 'Yes',
        'baker_balance': convertFromUtezToTez(result[0].balance),
        'baker_delegated_balance': convertFromUtezToTez(result[0].delegated_balance),
        'baker_frozen_balance': convertFromUtezToTez(result[0].frozen_balance),
        'baker_staking_balance': convertFromUtezToTez(result[0].staking_balance)
      };
    } else {
      item = {
        ...account[0],
        balance: convertFromUtezToTez(account[0].balance),
        'baker_deactivated': '',
        'baker_balance': '',
        'baker_delegated_balance': '',
        'baker_frozen_balance': '',
        'baker_staking_balance': ''
      };
    }
    dispatch(setItemsAction(entity, item));
    return item;
  } else {
    dispatch(setErrorAction(InvalidId, entity));
    return '';
  }
};

export const getOperationsThunk = (id: string) => async (dispatch, state) => {
  dispatch(setLoadingAction(true));
  const { selectedConfig } = state().app;
  const entity = 'operation';
  const { conseilServer, network } = getInfo(selectedConfig);
  const query = getQueryForOperations(id);
  const transactions = await TezosConseilClient.getOperations(conseilServer, network, query).catch(e => {
    console.error('transactions', e);
    return [];
  });
  if (transactions.length > 0) {
    const item = transactions.map(transaction => {
      return {
        ...transaction,
        amount: convertFromUtezToTez(transaction.amount),
        fee: convertFromUtezToTez(transaction.fee),
        'consumed_gas': convertFromUtezToTez(transaction.consumed_gas)
      };
    });
    dispatch(setItemsAction(entity, item));
    return item;
  } else {
    dispatch(setErrorAction(InvalidId, entity));
    return '';
  }
};

export const getBlockHashThunk = (level: number) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const { conseilServer, network } = getInfo(selectedConfig);
  const block = await TezosConseilClient.getBlockByLevel(conseilServer, network, level);
  if (block.length > 0) {
    return block[0].hash;
  } else {
    dispatch(setErrorAction(NoBlock, ''));
    return '';
  }
};

export const initLoadThunk = () => async (dispatch, state) => {
  dispatch(setLoadingAction(true));
  const { selectedConfig } = state().app;
  const { conseilServer, network } = getInfo(selectedConfig);
  const head = await TezosConseilClient.getBlockHead(conseilServer, network).catch(e => {
    console.log('error', e);
    return [];
  });
  if (head.length > 0) {
    return dispatch(getBlockThunk(head[0].hash));
  } else {
    dispatch(setErrorAction(NoAvaialbe, 'block'));
    return '';
  }
};

function openSharedUrl(query: any, displayName: string, entity: string) {
  const serializedQuery = JSON.stringify(query);
  const encodedUrl = base64url(serializedQuery);
  const shareLink = `${arronaxURL}?e=${encodeURIComponent(displayName)}/${encodeURIComponent(entity)}&q=${encodedUrl}`;
  window.open(shareLink, '_blank');
}

export const shareBlockLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForBlockLink(id);
  openSharedUrl(query, selectedConfig.displayName, 'operations');
};

export const shareAccountSendsLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForAccountSends(id);
  openSharedUrl(query, selectedConfig.displayName, 'operations');
};

export const shareAccountReceiptsLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForAccountReceipts(id);
  openSharedUrl(query, selectedConfig.displayName, 'operations');
};

export const shareAccountOtherOperationsLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForOtherOperations(id);
  openSharedUrl(query, selectedConfig.displayName, 'operations');
};

export const shareAccountEndorsementsLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForEndorsements(id);
  openSharedUrl(query, selectedConfig.displayName, 'operations');
};

export const shareAccountBakedBlocksLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForBakedBlocks(id);
  openSharedUrl(query, selectedConfig.displayName, 'blocks');
};

export const shareAccountDepositsLinkThunk = (id: string) => async (dispatch, state) => {
  const { selectedConfig } = state().app;
  const query = getQueryForDepositsAndRewards(id);
  openSharedUrl(query, selectedConfig.displayName, 'balance_updates');
};

