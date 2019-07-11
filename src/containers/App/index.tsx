import React from 'react';
import { connect } from 'react-redux';
import { compose } from 'redux';
import { Route, Redirect, Switch, RouteComponentProps, withRouter } from 'react-router-dom';
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
import { getLoading, getError, getIsError, getConfig, getEntity } from '../../reducers/app/selectors';
import { getBlockHashThunk, initLoadThunk } from '../../reducers/app/thunks';
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
  font-family: 'Arcade', sans-serif;
  font-size: 60px;
  padding: 45px 45px 30px 45px;
  color: white;
  line-height: 35px;
  letter-spacing: 3px;
  cursor: pointer;
`;
const MainContainer = styled.div`
  padding: 0 35px;
  flex: 1;
  margin-bottom: 40px;
`;

interface OwnProps {
  entity: string;
  isLoading: boolean;
  isError: boolean;
  error: string;
  selectedConfig: Config;
  getHash: (level: number) => string;
  removeError: () => void;
  setError: (error: string) => void;
  changeNetwork: (config: Config) => void;
  initLoad: () => void;
}

type Props = OwnProps & RouteComponentProps;

interface States {
  isOpenNetworkSelector: boolean;
}

class App extends React.Component<Props, States> {
  constructor(props) {
    super(props);
    this.state = {
      isOpenNetworkSelector: false
    };
  }

  onSearchById = async (val: string) => {
    const firstChar = val[0].toLowerCase();
    const twoChars = val.substring(0, 2).toLowerCase();
    const { history, getHash, setError } = this.props;
    if (firstChar === 'b') {
      history.push(`/blocks/${val}`);
    } else if (firstChar === 'o') {
      history.push(`/operations/${val}`);
    } else if (twoChars === 'tz' || twoChars === 'kt') {
      history.push(`/accounts/${val}`);
    } else if (Number(val)) {
      const hash = await getHash(Number(val));
      if (hash) {
        history.push(`/blocks/${hash}`);
      }
    } else {
      setError(InvalidId);
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
    const { history, entity, initLoad } = this.props;
    history.push('/');
    if (entity === 'block') {
      initLoad();
    }
  }

  render() {
    const { isLoading, isError, error, selectedConfig } = this.props;
    const { isOpenNetworkSelector } = this.state;
    return (
      <ThemeProvider theme={themes[selectedConfig.network]}>
        <Container>
          <Header onClick={this.gotoHome}>MININAX</Header>
          <MainContainer>
            <Switch>
              <Route exact path='/' component={Block} />} />
              <Route exact path='/blocks/:id' component={Block} />
              <Route exact path='/accounts/:id' component={Account} />
              <Route exact path='/operations/:id' component={Operation} />
              <Redirect to='/' push/>
            </Switch>
          </MainContainer>
          <Footer
            onSearch={this.onSearchById}
            network={selectedConfig.displayName}
            onOpenNetworkSelector={this.onOpenNetworkModal}
            gotoHome={this.gotoHome}
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
  entity: getEntity(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getHash: (level: number) => dispatch(getBlockHashThunk(level)),
  removeError: () => dispatch(removeErrorAction()),
  setError: (error: string) => dispatch(setErrorAction(error, '')),
  changeNetwork: (config: Config) => dispatch(changeNetworkAction(config)),
  initLoad: () => dispatch(initLoadThunk())
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(App);