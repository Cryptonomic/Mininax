import React from 'react';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';

const entity = 'account';

interface Props {
  items: any;
}

class Account extends React.Component<Props, {}> {
  render() {
    const { items } = this.props;
    return (
      <Container>
        <Title>Account</Title>
        {items[entity] &&
          <ItemDisplay entity={entity} item={items[entity]} />
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

export default connect(
  mapStateToProps,
  null
)(Account);