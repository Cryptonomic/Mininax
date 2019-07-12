import React from 'react';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, AccountBtn } from '../style';
import {
  shareAccountSendsLinkThunk,
  shareAccountReceiptsLinkThunk,
  shareAccountOtherOperationsLinkThunk,
  shareAccountEndorsementsLinkThunk,
  shareAccountBakedBlocksLinkThunk,
  shareAccountDepositsLinkThunk
} from '../../reducers/app/thunks';

const entity = 'account';

interface Props {
  items: any;
  openAccountSends: (id: string) => void;
  openAccountReceipts: (id: string) => void;
  openAccountOtherOperations: (id: string) => void;
  openAccountEndorsements: (id: string) => void;
  openAccountBakedBlocks: (id: string) => void;
  openAccountDeposits: (id: string) => void;
}

class Account extends React.Component<Props, {}> {
  onOpenAccountSends = () => {
    const { items, openAccountSends } = this.props;
    const id = items[entity].account_id;
    openAccountSends(id);
  }

  onOpenAccountReceipts = () => {
    const { items, openAccountReceipts } = this.props;
    const id = items[entity].account_id;
    openAccountReceipts(id);
  }

  onOpenAccountOtherOperations = () => {
    const { items, openAccountOtherOperations } = this.props;
    const id = items[entity].account_id;
    openAccountOtherOperations(id);
  }

  onOpenAccountEndorsements = () => {
    const { items, openAccountEndorsements } = this.props;
    const id = items[entity].account_id;
    openAccountEndorsements(id);
  }

  onOpenAccountBakedBlocks = () => {
    const { items, openAccountBakedBlocks } = this.props;
    const id = items[entity].account_id;
    openAccountBakedBlocks(id);
  }

  onOpenAccountDeposits = () => {
    const { items, openAccountDeposits } = this.props;
    const id = items[entity].account_id;
    openAccountDeposits(id);
  }

  render() {
    const { items } = this.props;
    return (
      <Container>
        <Title>Account</Title>
        {items[entity] &&
          <ItemDisplay entity={entity} item={items[entity]} />
        }
        <AccountBtn onClick={this.onOpenAccountSends}>
          All sent transactions
        </AccountBtn>
        <AccountBtn onClick={this.onOpenAccountReceipts}>
          All received transactions
        </AccountBtn>
        <AccountBtn onClick={this.onOpenAccountOtherOperations}>
          All reveals, delegations and originations
        </AccountBtn>
        <AccountBtn onClick={this.onOpenAccountEndorsements}>
          All endorsements
        </AccountBtn>
        <AccountBtn onClick={this.onOpenAccountBakedBlocks}>
          All baked blocks
        </AccountBtn>
        <AccountBtn onClick={this.onOpenAccountDeposits}>
          All deposits and rewards
        </AccountBtn>
      </Container>
    );
  }
}

const mapStateToProps = (state: any) => ({
  items: getItems(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  openAccountSends: (id: string) => dispatch(shareAccountSendsLinkThunk(id)),
  openAccountReceipts: (id: string) => dispatch(shareAccountReceiptsLinkThunk(id)),
  openAccountOtherOperations: (id: string) => dispatch(shareAccountOtherOperationsLinkThunk(id)),
  openAccountEndorsements: (id: string) => dispatch(shareAccountEndorsementsLinkThunk(id)),
  openAccountBakedBlocks: (id: string) => dispatch(shareAccountBakedBlocksLinkThunk(id)),
  openAccountDeposits: (id: string) => dispatch(shareAccountDepositsLinkThunk(id))
});

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Account);