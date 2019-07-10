import React from 'react';
import { match, RouteComponentProps } from 'react-router-dom';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getAccountThunk } from '../../reducers/app/thunks';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, LinkBtn } from '../style';

const entity = 'account';

interface MatchParams {
  id: string;
}

interface Props {
  items: any;
  match?: match<MatchParams>;
  getAccount: (id: string) => void;
}

class Account extends React.Component<Props, {}> {
  componentWillMount() {
    const {match, getAccount } = this.props;
    const id = match.params.id;
    if (id) {
      getAccount(id);
    }
  }

  componentDidUpdate(prevProps) {
    const { match, getAccount } = this.props;
    if (match.params.id && match.params.id !== prevProps.match.params.id) {
      getAccount(match.params.id);
    }
  }

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

const mapDispatchToProps = (dispatch: any) => ({
  getAccount: (id: string) => dispatch(getAccountThunk(id))
});

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Account);