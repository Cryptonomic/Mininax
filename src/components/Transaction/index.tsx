import React from 'react';
import {
  Container, Title, TitleContainer, TransactionBtn, TitleBtnContainer,
  MainContainer, FieldLabel, FieldContent, RowContainer, CopyImg
} from '../ItemDisplay/style';

import { getFields, copyContent } from '../../utils/general';
import { Field } from '../../types';
import copySvg from '../../assets/img/copy-yellow.svg';

export interface Props {
  entity: string;
  item: any;
}

const Transaction: React.FC<Props> = (props) => {
  const { entity, item } = props;
  const fields = getFields(entity);
  const [count, setCount] = React.useState(0);
  const total = item.length;

  function onCopy(val: string) {
    copyContent(val);
  }

  return (
    <Container>
      <TitleContainer>
        <Title>Transaction ({count + 1}/{total})</Title>
        <TitleBtnContainer>
          <TransactionBtn disabled={count === 0} onClick={() => setCount(count - 1)}>{'<'}</TransactionBtn>
          <TransactionBtn disabled={count === total - 1} onClick={() => setCount(count + 1)}>{'>'}</TransactionBtn>
        </TitleBtnContainer>
      </TitleContainer>
      <MainContainer>
        {fields.map((field: Field, index: number) => {
          if (field.name === 'parameters') {
            return (
              <RowContainer key={field.name}>
                <FieldLabel isBar={index === 0 || index === fields.length - 1}>
                  {field.displayName}
                  <CopyImg src={copySvg} onClick={() => onCopy(item[count][field.name])} />
                </FieldLabel>
                <FieldContent>{item[count][field.name]}</FieldContent>
              </RowContainer>
            );
          }

          return (
            <RowContainer key={field.name} isBottom={index === fields.length - 1}>
              <FieldLabel isBar={index === 0 || index === fields.length - 1}>{field.displayName}</FieldLabel>
              <FieldContent>{item[count][field.name]}</FieldContent>
            </RowContainer>
          );
        })}
      </MainContainer>
    </Container>
  );
};

export default Transaction;