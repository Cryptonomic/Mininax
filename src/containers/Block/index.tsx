import React from 'react';
import { compose } from 'redux';
import { match, RouteComponentProps, withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getBlockThunk, getBlockHashThunk, initLoadThunk } from '../../reducers/app/thunks';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';

const entity = 'block';

interface MatchParams {
  id: string;
}

interface OwnProps {
  items: any;
  match?: match<MatchParams>;
  getBlock: (id: string) => void;
  initLoad: () => void;
  getHash: (level: number) => string;
}

type Props = OwnProps & RouteComponentProps;

class Block extends React.Component<Props, {}> {
  componentWillMount() {
    const { initLoad, match, getBlock } = this.props;
    const id = match.params.id;
    if (id) {
      getBlock(id);
    } else {
      initLoad();
    }
  }

  componentDidUpdate(prevProps) {
    const { match, getBlock } = this.props;
    if (match.params.id && match.params.id !== prevProps.match.params.id) {
      getBlock(match.params.id);
    }
  }

  getHashByLevel = async (level: number, inc: number) => {
    const { getHash, history } = this.props;
    const hash = await getHash(level + inc);
    console.log('hash', hash);
    if (hash) {
      history.push(`/blocks/${hash}`);
    }
  }

  render() {
    const { items } = this.props;
    return (
      <Container>
        <Title>Block</Title>
        {items[entity] &&
          <ItemDisplay entity={entity} item={items[entity]} changeLevel={this.getHashByLevel} />
        }
        <LinkBtn>
          All Operations >>
        </LinkBtn>
      </Container>
    );
  }
}

const mapStateToProps = (state: any) => ({
  items: getItems(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getBlock: (id: string) => dispatch(getBlockThunk(id)),
  initLoad: () => dispatch(initLoadThunk()),
  getHash: (level: number) => dispatch(getBlockHashThunk(level))
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(Block);