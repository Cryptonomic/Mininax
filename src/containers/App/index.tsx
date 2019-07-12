import React from 'react';
import { connect } from 'react-redux';
import { compose } from 'redux';
import { Route, Redirect, Switch, RouteComponentProps, withRouter, match } from 'react-router-dom';
import styled, { ThemeProvider } from 'styled-components';
import Block from '../Block';
import Account from '../Account';
import Operation from '../Operation';
import Footer from '../../components/Footer';
import Loader from '../../components/Loader';
import Error from '../../components/Error';
import NetworkSelector from '../../components/NetworkSelector';
import { Config } from '../../types';
import themes from '../../utils/themes';
import { getLoading, getError, getIsError, getConfig, getConfigs } from '../../reducers/app/selectors';
import { getBlockHashThunk, initLoadThunk, getBlockThunk, getAccountThunk, getOperationsThunk } from '../../reducers/app/thunks';
import { removeErrorAction, setErrorAction, changeNetworkAction } from '../../reducers/app/actions';

import '../../assets/scss/App.scss';

const InvalidId = 'You entered an invalid ID.';

const Container = styled.div`
  width: 100%;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background-color: ${props => props.theme.mainBgColor};
`;
const Header = styled.div`
  padding: 45px 45px 30px 45px;
`;
const HeaderTitle = styled.div`
  font-family: 'Arcade', sans-serif;
  font-size: 60px;
  color: white;
  line-height: 35px;
  letter-spacing: 3px;
  cursor: pointer;
  display: inline-block;
`;
const MainContainer = styled.div`
  padding: 0 35px;
  flex: 1;
  margin-bottom: 40px;
`;

interface MatchParams {
  network: string;
  id: string;
}

interface OwnProps {
  isLoading: boolean;
  isError: boolean;
  error: string;
  selectedConfig: Config;
  configs: Config[];
  match?: match<MatchParams>;
  location: any;
  getHash: (level: number) => string;
  removeError: () => void;
  setError: (error: string) => void;
  changeNetwork: (config: Config) => void;
  initLoad: () => any;
  getBlock: (id: string) => any;
  getOperation: (id: string) => any;
  getAccount: (id: string) => any;
}

type Props = OwnProps & RouteComponentProps;

interface States {
  isOpenNetworkSelector: boolean;
}

class App extends React.Component<Props, States> {
  footerRef: any = null;
  constructor(props) {
    super(props);
    this.state = {
      isOpenNetworkSelector: false
    };

    this.footerRef = React.createRef();
  }

  componentWillMount = async () => {
    const { initLoad, configs, changeNetwork, location } = this.props;
    const paths = location.pathname.split('/');
    const network = paths[1];
    const id = paths[3];

    if (id) {
      const selectedConfig = configs.find(conf => conf.network === network);
      if (selectedConfig) {
        await changeNetwork(selectedConfig);
        this.onSearchById(id);
      } else {
        initLoad();
      }
    } else {
      initLoad();
    }
  }

  onSearchById = async (val: string) => {
    const firstChar = val[0].toLowerCase();
    const twoChars = val.substring(0, 2).toLowerCase();
    const { history, getHash, setError, getBlock, getAccount, getOperation, selectedConfig } = this.props;
    let item = null;
    let route = '';
    if (firstChar === 'b') {
      item = await getBlock(val);
      route = `/${selectedConfig.network}/blocks/${val}`;
    } else if (firstChar === 'o') {
      item = await getOperation(val);
      route = `/${selectedConfig.network}/operations/${val}`;
    } else if (twoChars === 'tz' || twoChars === 'kt') {
      item = await getAccount(val);
      route = `/${selectedConfig.network}/accounts/${val}`;
    } else if (Number(val)) {
      const hash = await getHash(Number(val));
      if (hash) {
        item = await getBlock(val);
        route = `/${selectedConfig.network}/blocks/${hash}`;
      }
    } else {
      setError(InvalidId);
    }

    if (item) {
      history.push(route);
    }
  }

  onRemoveErrorModal = () => {
    const { removeError } = this.props;
    removeError();
  }
  onRemoveNetworkModal = () => this.setState({isOpenNetworkSelector: false});
  onOpenNetworkModal = () => this.setState({isOpenNetworkSelector: true});

  onChangeNetwork = async (config: Config) => {
    const { selectedConfig, changeNetwork } = this.props;
    if (config.network !== selectedConfig.network) {
      await changeNetwork(config);
      this.gotoHome();
    }
    this.setState({isOpenNetworkSelector: false});
  }

  gotoHome = () => {
    const { history, initLoad } = this.props;
    history.push('/');
    initLoad();
  }

  onKeyEvent = (keyVal) => {
    if (keyVal.toLowerCase() === 's') {
      this.footerRef.current.focusSearch();
    }
  }

  render() {
    const { isLoading, isError, error, selectedConfig } = this.props;
    const { isOpenNetworkSelector } = this.state;
    return (
      <ThemeProvider theme={themes[selectedConfig.network]}>
        <Container tabIndex={0} onKeyPress={event => this.onKeyEvent(event.key)}>
          <Header>
            <HeaderTitle onClick={this.gotoHome}>MININAX</HeaderTitle>
          </Header>
          <MainContainer>
            <Switch>
              <Route exact path='/' component={Block} />
              <Route exact path='/:network/blocks/:id' component={Block} />
              <Route exact path='/:network/accounts/:id' component={Account} />
              <Route exact path='/:network/operations/:id' component={Operation} />
              <Redirect to='/' push/>
            </Switch>
          </MainContainer>
          <Footer
            ref={this.footerRef}
            onSearch={this.onSearchById}
            network={selectedConfig.displayName}
            onOpenNetworkSelector={this.onOpenNetworkModal}
          />
          {isLoading &&  <Loader />}
          {isError && <Error error={error} onTry={this.onRemoveErrorModal} />}
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

const mapStateToProps = (state: any) => ({
  isLoading: getLoading(state),
  error: getError(state),
  isError: getIsError(state),
  selectedConfig: getConfig(state),
  configs: getConfigs(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getHash: (level: number) => dispatch(getBlockHashThunk(level)),
  removeError: () => dispatch(removeErrorAction()),
  setError: (error: string) => dispatch(setErrorAction(error, '')),
  changeNetwork: (config: Config) => dispatch(changeNetworkAction(config)),
  initLoad: () => dispatch(initLoadThunk()),
  getBlock: (id: string) => dispatch(getBlockThunk(id)),
  getOperation: (id: string) => dispatch(getOperationsThunk(id)),
  getAccount: (id: string) => dispatch(getAccountThunk(id))
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(App);