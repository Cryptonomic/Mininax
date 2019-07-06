import React from 'react';
import { hot } from 'react-hot-loader';
import styled, { ThemeProvider } from 'styled-components';
import {  TezosConseilClient, ConseilDataClient } from 'conseiljs';
import Footer from '../components/Footer';
import ItemDisplay from '../components/ItemDisplay';
import Transaction from '../components/Transaction';
import Loader from '../components/Loader';
import Error from '../components/Error';
import NetworkSelector from '../components/NetworkSelector';
import configs from '../config';
import { Config } from '../types';
import { getQueryForBlockFee, convertFromUtezToTez, getQueryForTransactions } from '../utils/general';
import themes from '../utils/themes';

import './../assets/scss/App.scss';

const Container = styled.div`
  width: 100%;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background-color: ${props => props.theme.mainBgColor};
`;
const Header = styled.div`
  font-family: 'Arcade', sans-serif;
  font-size: 60px;
  padding: 45px 45px 30px 45px;
  color: white;
  line-height: 35px;
  letter-spacing: 3px;
`;
const MainContainer = styled.div`
  padding: 0 35px;
  flex: 1;
  margin-bottom: 40px;
`;

interface Props {
}

interface States {
  entity: string;
  selectedConfig: Config;
  item: any;
  isLoading: boolean;
  isError: boolean;
  isOpenNetworkSelector: boolean;
}

class App extends React.Component<Props, States> {
  constructor(props) {
    super(props);
    this.state = {
      entity: '',
      selectedConfig: configs[0],
      item: {},
      isLoading: false,
      isError: false,
      isOpenNetworkSelector: false
    };
  }

  getInfo = () => {
    const { selectedConfig } = this.state;
    const { platform, network, url, apiKey } = selectedConfig;
    const conseilServer = { url, apiKey };
    return { conseilServer, platform, network };
  }
  componentWillMount() {
    this.initLoad();
  }

  initLoad = async () => {
    const { conseilServer, network } = this.getInfo();
    const head = await TezosConseilClient.getBlockHead(conseilServer, network);
    this.getBlock(head[0].hash);
  }

  getBlock = async (id) => {
    this.setState({isLoading: true});
    const { conseilServer, network, platform } = this.getInfo();
    const query = getQueryForBlockFee(id);
    const block = await TezosConseilClient.getBlock(conseilServer, network, id).catch(e => {
      console.error('block', e);
      this.setState({isLoading: false, isError: true});
      return [];
    });
    if (block.length > 0) {
      const fees = await ConseilDataClient.executeEntityQuery(conseilServer, platform, network, 'operations', query);
      const item = {
        ...block[0],
        'consumed_gas': convertFromUtezToTez(block[0].consumed_gas),
        fee: convertFromUtezToTez(fees[0].sum_fee),
        'baker_priority': 'Coming soon'
      };
      this.setState({entity: 'block', isLoading: false, item});
    } else {
      this.setState({isLoading: false, isError: true});
    }
  }

  getAccount = async (id: string) => {
    this.setState({isLoading: true});
    const { conseilServer, network } = this.getInfo();
    const account = await TezosConseilClient.getAccount(conseilServer, network, id).catch(e => {
      console.error('account', e);
      this.setState({isLoading: false, isError: true});
      return [];
    });
    if (account.length > 0) {
      const item = {
        ...account[0],
        balance: convertFromUtezToTez(account[0].balance)
      };
      this.setState({entity: 'account', isLoading: false, item});
    } else {
      this.setState({isLoading: false, isError: true});
    }
  }

  getTransactions = async (id: string) => {
    this.setState({isLoading: true});
    const { conseilServer, network } = this.getInfo();
    const query = getQueryForTransactions(id);
    const transactions = await TezosConseilClient.getOperations(conseilServer, network, query).catch(e => {
      console.error('transactions', e);
      this.setState({isLoading: false, isError: true});
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
      this.setState({entity: 'transaction', isLoading: false, item});
    } else {
      this.setState({isLoading: false, isError: true});
    }
  }

  updateBlockByLevel = async (level: string | number, inc: number) => {
    const { conseilServer, network } = this.getInfo();
    const block = await TezosConseilClient.getBlockByLevel(conseilServer, network, Number(level) + inc);
    if (block.length > 0) {
      const blockHash = block[0].hash;
      this.getBlock(blockHash);
    } else {
      // this.setState({isLoading: false, isError: true});
    }
  }

  onSearchById = (val: string) => {
    const firstChar = val[0].toLowerCase();
    const twoChars = val.substring(0, 2).toLowerCase();
    if (firstChar === 'b') {
      this.getBlock(val);
    } else if (firstChar === 'o') {
      this.getTransactions(val);
    } else if (twoChars === 'tz' || twoChars === 'kt') {
      this.getAccount(val);
    }
  }

  onRemoveErrorModal = () => this.setState({isError: false});
  onRemoveNetworkModal = () => this.setState({isOpenNetworkSelector: false});
  onOpenNetworkModal = () => this.setState({isOpenNetworkSelector: true});

  onChangeNetwork = (config: Config) => {
    const { selectedConfig } = this.state;
    if (config.network !== selectedConfig.network) {
      this.setState({selectedConfig: config, isOpenNetworkSelector: false});
      setTimeout(() => {
        this.initLoad();
      }, 100);
    } else {
      this.setState({isOpenNetworkSelector: false});
    }
  }

  render() {
    const { entity, item, selectedConfig, isLoading, isError, isOpenNetworkSelector } = this.state;
    return (
      <ThemeProvider theme={themes[selectedConfig.network]}>
        <Container>
          <Header>MININAX</Header>
          <MainContainer>
            {entity && entity !== 'transaction' && <ItemDisplay entity={entity} item={item} changeLevel={this.updateBlockByLevel} />}
            {entity === 'transaction' && <Transaction entity={entity} item={item} />}
          </MainContainer>
          <Footer onSearch={this.onSearchById} network={selectedConfig.displayName} onOpenNetworkSelector={this.onOpenNetworkModal} />
          {isLoading &&  <Loader />}
          {isError && <Error onTry={this.onRemoveErrorModal} />}
          {isOpenNetworkSelector &&
            <NetworkSelector
              selectedConfig={selectedConfig}
              onChange={this.onChangeNetwork}
              onCancel={this.onRemoveNetworkModal}
            />
          }
        </Container>
      </ThemeProvider>
    );
  }
}

declare let module: Object;

export default hot(module)(App);