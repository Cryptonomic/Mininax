import React from 'react';
import { compose } from 'redux';
import { RouteComponentProps, withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getBlockThunk, getBlockHashThunk } from '../../reducers/app/thunks';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';

const entity = 'block';

interface OwnProps {
  items: any;
  getBlock: (id: string) => any;
  getHash: (level: number) => string;
}

type Props = OwnProps & RouteComponentProps;

class Block extends React.Component<Props, {}> {
  getHashByLevel = async (level: number, inc: number) => {
    const { getHash, history, getBlock } = this.props;
    const hash = await getHash(level + inc);
    if (hash) {
      const block = await getBlock(hash);
      if (block) {
        history.push(`/blocks/${hash}`);
      }
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
  getHash: (level: number) => dispatch(getBlockHashThunk(level))
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(Block);