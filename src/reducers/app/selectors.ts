import { createSelector } from 'reselect';
import { RootState } from '../index';

export const getAppState = (state: RootState) => state.app;

export const getItems = createSelector(
  getAppState,
  apps => apps.items
);

export const getLoading = createSelector(
  getAppState,
  apps => apps.isLoading
);

export const getError = createSelector(
  getAppState,
  apps => apps.error
);

export const getIsError = createSelector(
  getAppState,
  apps => apps.isError
);

export const getConfig = createSelector(
  getAppState,
  apps => apps.selectedConfig
);
