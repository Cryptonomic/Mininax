import React from 'react';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { shareBlockLinkThunk } from '../../reducers/app/thunks';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';

const entity = 'block';

interface Props {
  items: any;
  shareUrl: (id: string) => void;
  goToDetail: (id: string) => void;
  goToBlock: (level: number) => void;
}

class Block extends React.Component<Props, {}> {
  onOpenUrl = () => {
    const { items, shareUrl } = this.props;
    const id = items[entity].hash;
    shareUrl(id);
  }

  render() {
    const { items, goToDetail, goToBlock } = this.props;
    return (
      <Container>
        <Title>Block</Title>
        {items[entity] &&
          <ItemDisplay entity={entity} item={items[entity]} changeLevel={goToBlock} goToDetail={goToDetail} />
        }
        <LinkBtn onClick={this.onOpenUrl}>
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
  shareUrl: (id: string) => dispatch(shareBlockLinkThunk(id))
});

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Block);