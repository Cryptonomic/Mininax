import React from 'react';
import { match } from 'react-router-dom';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getOperationsThunk } from '../../reducers/app/thunks';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, TransactionBtn, TitleBtnContainer, TitleContainer } from '../style';

const entity = 'operation';
const Titles = {
  transaction: 'Transaction',
  'activate_account': 'Activate Account',
  reveal: 'Reveal',
  delegation: 'Delegation',
  origination: 'Origination'
};

interface MatchParams {
  id: string;
}

interface Props {
  items: any;
  match?: match<MatchParams>;
  getOperation: (id: string) => void;
}

interface States {
  count: number;
}

class Operation extends React.Component<Props, States> {
  constructor(props) {
    super(props);
    this.state = {
      count: 0
    };
    const { match, getOperation } = props;
    const id = match.params.id;
    if (id) {
      getOperation(id);
    }
  }

  componentDidUpdate(prevProps) {
    const { match, getOperation } = this.props;
    if (match.params.id && match.params.id !== prevProps.match.params.id) {
      getOperation(match.params.id);
    }
  }

  changeCount = (count: number) => {
    this.setState({count});
  }

  render() {
    const { items } = this.props;
    const { count } = this.state;
    const operations = items[entity];
    const total = operations ? operations.length : 0;
    return (
      <Container>
        {total > 1 ?
          (
            <TitleContainer>
              <Title>Operation ({count + 1}/{total})</Title>
              <TitleBtnContainer>
                <TransactionBtn disabled={count === 0} onClick={() => this.changeCount(count - 1)}>{'<'}</TransactionBtn>
                <TransactionBtn disabled={count === total - 1} onClick={() => this.changeCount(count + 1)}>{'>'}</TransactionBtn>
              </TitleBtnContainer>
            </TitleContainer>
          ) :
          (<Title>Operation</Title>)
        }
        {operations && <ItemDisplay entity={entity} kind={operations[count].kind} item={operations[count]} />}
      </Container>
    );
  }
}

const mapStateToProps = (state: any) => ({
  items: getItems(state)
});

const mapDispatchToProps = (dispatch: any) => ({
  getOperation: (id: string) => dispatch(getOperationsThunk(id))
});

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Operation);