import React from 'react';
import { connect } from 'react-redux';
import ItemDisplay from '../../components/ItemDisplay';
import { getItems } from '../../reducers/app/selectors';
import { Container, Title, TransactionBtn, TitleBtnContainer, TitleContainer } from '../style';

const entity = 'operation';
interface Props {
  items: any;
  goToDetail?: (id: string) => void;
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
  }

  changeCount = (count: number) => {
    this.setState({count});
  }

  render() {
    const { items, goToDetail } = this.props;
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
        {operations && <ItemDisplay entity={entity} kind={operations[count].kind} item={operations[count]} goToDetail={goToDetail} />}
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
)(Operation);