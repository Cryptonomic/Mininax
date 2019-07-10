import { TezosConseilClient, ConseilDataClient } from 'conseiljs';
import { setLoadingAction, setItemsAction, setErrorAction } from './actions';
import { Config } from '../../types';
import { getQueryForBlockFee, convertFromUtezToTez, getQueryForOperations } from '../../utils/general';

const InvalidId = 'You entered an invalid ID.';
const NoBlock = 'There is no block for the Level.';
const NoAvaialbe = 'Not available';

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
  const query = getQueryForBlockFee(id);
  const block = await TezosConseilClient.getBlock(conseilServer, network, id).catch(e => {
    console.error('block', e);
    return [];
  });
  if (block.length > 0) {
    const fees = await ConseilDataClient.executeEntityQuery(conseilServer, platform, network, 'operations', query).catch(e => {
      return [];
    });

    if (fees.length > 0) {
      const item = {
        ...block[0],
        'consumed_gas': convertFromUtezToTez(block[0].consumed_gas),
        fee: convertFromUtezToTez(fees[0].sum_fee),
        'baker_priority': 'Coming soon'
      };
      dispatch(setItemsAction(entity, item));
    } else {
      dispatch(setErrorAction(NoAvaialbe, entity));
    }
  } else {
    dispatch(setErrorAction(InvalidId, entity));
  }
};

export const getAccountThunk = (id: string) => async (dispatch, state) => {
  dispatch(setLoadingAction(true));
  const { selectedConfig } = state().app;
  const entity = 'account';
  const { conseilServer, network } = getInfo(selectedConfig);
  const account = await TezosConseilClient.getAccount(conseilServer, network, id).catch(e => {
    console.error('account', e);
    return [];
  });
  if (account.length > 0) {
    const item = {
      ...account[0],
      balance: convertFromUtezToTez(account[0].balance)
    };
    dispatch(setItemsAction(entity, item));
  } else {
    dispatch(setErrorAction(InvalidId, entity));
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
  console.log('transactions', transactions);
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
  } else {
    dispatch(setErrorAction(InvalidId, entity));
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
    dispatch(getBlockThunk(head[0].hash));
  } else {
    dispatch(setErrorAction(NoAvaialbe, 'block'));
  }
};

