import React from 'react';
import {
  MainContainer, FieldLabel, FieldContent, RowContainer,
  LevelLabelContainer, LevelBtn, LevelLabel, CopyImg
} from './style';

import { getFields, copyContent } from '../../utils/general';
import { Field } from '../../types';
import copySvg from '../../assets/img/copy-yellow.svg';

const options = { month: 'long', day: 'numeric', year: 'numeric', hour: 'numeric', minute: 'numeric', timeZone: 'UTC', timeZoneName: 'short' };

interface Props {
  entity: string;
  item: any;
  changeLevel?(level: string | number, inc: number): void;
}


const ItemDisplay: React.FC<Props> = (props) => {
  const { entity, item, changeLevel } = props;
  const fields = getFields(entity);
  function onCopy(val: string) {
    copyContent(val);
  }
  return (
      <MainContainer>
        {fields.map((field: Field, index: number) => {
          if (field.name === 'level') {
            return (
              <RowContainer key={field.name} isBottom={index === fields.length - 1}>
                <LevelLabelContainer>
                  <LevelBtn onClick={() => changeLevel(item[field.name], -1)}>{'<'}</LevelBtn>
                  <LevelLabel>Level</LevelLabel>
                  <LevelBtn onClick={() => changeLevel(item[field.name], 1)}>{'>'}</LevelBtn>
                </LevelLabelContainer>
                <FieldContent>{item[field.name]}</FieldContent>
              </RowContainer>
            );
          }
          if (field.name === 'script' || field.name === 'storage' || field.name === 'parameters') {
            return (
              <RowContainer key={field.name} isBottom={index === fields.length - 1}>
                <FieldLabel isBar={index === 0 || index === fields.length - 1}>
                  {field.displayName}
                  <CopyImg src={copySvg} onClick={() => onCopy(item[field.name])} />
                </FieldLabel>
                <FieldContent>{item[field.name]}</FieldContent>
              </RowContainer>
            );
          }

          if (field.name === 'timestamp') {
            return (
              <RowContainer key={field.name} isBottom={index === fields.length - 1}>
                <FieldLabel>{field.displayName}</FieldLabel>
                <FieldContent>{new Intl.DateTimeFormat('en-US', options).format(item[field.name])}</FieldContent>
              </RowContainer>
            );
          }

          return (
            <RowContainer key={field.name} isBottom={index === fields.length - 1}>
              <FieldLabel isBar={index === 0 || index === fields.length - 1}>{field.displayName}</FieldLabel>
              <FieldContent>{item[field.name]}</FieldContent>
            </RowContainer>
          );
        })}
      </MainContainer>
  );
};

export default ItemDisplay;