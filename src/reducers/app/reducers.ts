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
import configs from '../../config';


export interface AppState {
  entity: string;
  id: string;
  isLoading: boolean;
  items: any;
  configs: Config[];
  selectedConfig: Config;
  isError: boolean;
  error: string;
}

let initialState: AppState = {
  entity: '',
  id: '',
  isLoading: false,
  items: {},
  configs,
  selectedConfig: configs[0],
  isError: false,
  error: ''
};

export const app = (state = initialState, action) => {
  switch (action.type) {
    case SET_ITEMS: {
      const items = { ...state.items, [action.entity]: action.items };
      return { ...state, items, entity: action.entity, isLoading: false };
    }
    case SET_LOADING:
      return { ...state, isLoading: action.isLoading };
    case SET_ERROR: {
      return { ...state, isError: true, error: action.error, isLoading: false };
    }
    case REMOVE_ERROR:
      return { ...state, isError: false, error: '' };
    case CHANGE_NETWORK:
      return { ...state, selectedConfig: action.config };
    case SET_PARAMS:
      return { ...state, entity: action.entity, id: action.id };
    case SET_ID:
      return { ...state, id: action.id };
  }
  return state;
};
