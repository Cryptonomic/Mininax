import { Config } from './types';

const configs: Config[] = [
  {
    platform: 'tezos',
    network: 'mainnet',
    displayName: 'Tezos Mainnet',
    url: 'https://conseil-prod.cryptonomic-infra.tech:443',
    apiKey: 'galleon'
  },
  {
    platform: 'tezos',
    network: 'alphanet',
    displayName: 'Tezos Alphanet',
    url: 'https://conseil-dev.cryptonomic-infra.tech:443',
    apiKey: 'hooman'
  }
];

export default configs;
