import React from 'react';
import { connect } from 'react-redux';
import { compose } from 'redux';
import { Route, Redirect, Switch, RouteComponentProps, withRouter } from 'react-router-dom';
import styled, { ThemeProvider } from 'styled-components';
import RouterWrapper from '../RouterWrapper';
import Footer from '../../components/Footer';
import Loader from '../../components/Loader';
import Error from '../../components/Error';
import NetworkSelector from '../../components/NetworkSelector';
import { Config } from '../../types';
import themes from '../../utils/themes';
import { getLoading, getError, getIsError, getConfig, getConfigs, getEntity, getId } from '../../reducers/app/selectors';
import { getBlockHashThunk, initLoadThunk, getBlockThunk, getAccountThunk, getOperationsThunk } from '../../reducers/app/thunks';
import { removeErrorAction, setErrorAction, changeNetworkAction, setParamsAction, setIdAction } from '../../reducers/app/actions';
import { MatchParams } from '../../types';
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

interface OwnProps {
  isLoading: boolean;
  isError: boolean;
  error: string;
  selectedConfig: Config;
  configs: Config[];
  location: any;
  entity: string;
  id: string;
  getHash: (level: number) => string;
  removeError: () => void;
  setError: (error: string) => void;
  changeNetwork: (config: Config) => void;
  initLoad: () => any;
  getBlock: (id: string) => any;
  getOperation: (id: string) => any;
  getAccount: (id: string) => any;
  setParams: (entity: string, id: string) => void;
  setId: (id: string) => void;
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

  initData = async (params: MatchParams) => {
    const { id, network, entity } = params;
    const { initLoad, configs, changeNetwork, setParams, history } = this.props;
    if (!id && !network && !entity) {
      initLoad();
    } else if (!id || !network || !entity) {
      history.push('/');
    } else {
      const selectedConfig = configs.find(conf => conf.network === network);
      if (selectedConfig) {
        await changeNetwork(selectedConfig);
        await setParams(entity, id);
        this.loadData();
      } else {
        this.gotoHome();
      }
    }
  }

  loadData = async () => {
    const { entity, id, getBlock, getOperation, getAccount} = this.props;
    if (entity === 'blocks') {
      await getBlock(id);
    } else if (entity === 'operations') {
      await getOperation(id);
    } else if (entity === 'accounts') {
      await getAccount(id);
    } else {
      this.gotoHome();
    }
  }

  onSearchById = async (val: string) => {
    const firstChar = val[0].toLowerCase();
    const twoChars = val.substring(0, 2).toLowerCase();
    const { history, getHash, setError, getBlock, getAccount, getOperation, selectedConfig, setParams } = this.props;
    let item = null;
    let id = val;
    let entity = '';
    if (firstChar === 'b') {
      item = await getBlock(val);
      entity = 'blocks';
    } else if (firstChar === 'o') {
      item = await getOperation(val);
      entity = 'operations';
    } else if (twoChars === 'tz' || twoChars === 'kt') {
      item = await getAccount(val);
      entity = 'accounts';
    } else if (Number(val)) {
      const hash = await getHash(Number(val));
      if (hash) {
        item = await getBlock(val);
        entity = 'blocks';
        id = hash;
      }
    } else {
      setError(InvalidId);
    }
    if (item) {
      const route = `/${selectedConfig.network}/${entity}/${id}`;
      setParams(entity, id);
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

  gotoHome = async () => {
    const { history, initLoad, setParams } = this.props;
    await setParams('', '');
    history.push('/');
    initLoad();
  }

  onKeyEvent = (keyVal) => {
    if (keyVal.toLowerCase() === 's') {
      this.footerRef.current.focusSearch();
    }
  }

  goToBlockByLevel = async (level: number) => {
    const { getHash, history, getBlock, selectedConfig } = this.props;
    const hash = await getHash(level);
    if (hash) {
      const block = await getBlock(hash);
      if (block) {
        history.push(`/${selectedConfig.network}/blocks/${hash}`);
      }
    }
  }

  onChangeId = (id: string) => {
    const { setId } = this.props;
    setId(id);
  }

  render() {
    const { isLoading, isError, error, selectedConfig, id } = this.props;
    const { isOpenNetworkSelector } = this.state;
    return (
      <ThemeProvider theme={themes[selectedConfig.network]}>
        <Container tabIndex={0} onKeyPress={event => this.onKeyEvent(event.key)}>
          <Header>
            <HeaderTitle onClick={this.gotoHome}>MININAX</HeaderTitle>
          </Header>
          <MainContainer>
            <Switch>
            <Route exact path='/:network/:entity/:id' render={props => <RouterWrapper initData={this.initData} goToDetail={this.onSearchById} goToBlock={this.goToBlockByLevel} {...props} />} />
              <Route exact path='/' render={props => <RouterWrapper initData={this.initData} goToDetail={this.onSearchById} goToBlock={this.goToBlockByLevel} {...props} />} />
              <Redirect to='/' push/>
            </Switch>
          </MainContainer>
          <Footer
            ref={this.footerRef}
            id={id}
            onChangeId={this.onChangeId}
            onSearch={this.onSearchById}
            network={selectedConfig.displayName}
            onOpenNetworkSelector={this.onOpenNetworkModal}
          />
          {isLoading &&  <Loader />}
          {isError && <Error error={error} onCancel={this.onRemoveErrorModal} onTry={this.loadData} />}
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
  configs: getConfigs(state),
  entity: getEntity(state),
  id: getId(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getHash: (level: number) => dispatch(getBlockHashThunk(level)),
  removeError: () => dispatch(removeErrorAction()),
  setError: (error: string) => dispatch(setErrorAction(error, '')),
  changeNetwork: (config: Config) => dispatch(changeNetworkAction(config)),
  initLoad: () => dispatch(initLoadThunk()),
  getBlock: (id: string) => dispatch(getBlockThunk(id)),
  getOperation: (id: string) => dispatch(getOperationsThunk(id)),
  getAccount: (id: string) => dispatch(getAccountThunk(id)),
  setParams: (entity: string, id: string) => dispatch(setParamsAction(entity, id)),
  setId: (id: string) => dispatch(setIdAction(id))
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(App);