import React from 'react';
import { compose } from 'redux';
import { RouteComponentProps, withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getBlockThunk, getBlockHashThunk, shareBlockLinkThunk } from '../../reducers/app/thunks';
import { getItems, getConfig } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';
import { Config } from '../../types';

const entity = 'block';

interface OwnProps {
  items: any;
  selectedConfig: Config;
  getBlock: (id: string) => any;
  getHash: (level: number) => string;
  shareUrl: (id: string) => void;
}

type Props = OwnProps & RouteComponentProps;

class Block extends React.Component<Props, {}> {
  getHashByLevel = async (level: number, inc: number) => {
    const { getHash, history, getBlock, selectedConfig } = this.props;
    const hash = await getHash(level + inc);
    if (hash) {
      const block = await getBlock(hash);
      if (block) {
        history.push(`/${selectedConfig.network}/blocks/${hash}`);
      }
    }
  }

  onOpenUrl = () => {
    const { items, shareUrl } = this.props;
    const id = items[entity].hash;
    shareUrl(id);
  }

  render() {
    const { items } = this.props;
    return (
      <Container>
        <Title>Block</Title>
        {items[entity] &&
          <ItemDisplay entity={entity} item={items[entity]} changeLevel={this.getHashByLevel} />
        }
        <LinkBtn onClick={this.onOpenUrl}>
          All Operations >>
        </LinkBtn>
      </Container>
    );
  }
}

const mapStateToProps = (state: any) => ({
  items: getItems(state),
  selectedConfig: getConfig(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getBlock: (id: string) => dispatch(getBlockThunk(id)),
  getHash: (level: number) => dispatch(getBlockHashThunk(level)),
  shareUrl: (id: string) => dispatch(shareBlockLinkThunk(id))
});

export default compose(
  withRouter,
  connect(
    mapStateToProps,
    mapDispatchToProps
  )
)(Block);