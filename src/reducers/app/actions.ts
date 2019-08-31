import {
  SET_ITEMS,
  SET_LOADING,
  SET_ERROR,
  REMOVE_ERROR,
  CHANGE_NETWORK,
  SET_ID,
  SET_PARAMS
} from './types';

import { Config } from '../../types';

export function setItemsAction(entity: string, items: any) {
  return {
    type: SET_ITEMS,
    entity,
    items
  };
}

export function setLoadingAction(isLoading: boolean) {
  return {
    type: SET_LOADING,
    isLoading
  };
}

export function setErrorAction(error: string, entity: string) {
  return {
    type: SET_ERROR,
    error,
    entity
  };
}

export function removeErrorAction() {
  return {
    type: REMOVE_ERROR
  };
}

export function changeNetworkAction(config: Config) {
  return {
    type: CHANGE_NETWORK,
    config
  };
}

export function setParamsAction(entity: string, id: string) {
  return {
    type: SET_PARAMS,
    entity,
    id
  };
}

export function setIdAction(id: string) {
  return {
    type: SET_ID,
    id
  };
}