import React from 'react';
import {
  MainContainer, FieldLabel, FieldContent, RowContainer,
  LevelLabelContainer, LevelBtn, LevelLabel, CopyImg, LinkContent
} from './style';

import { getFields, copyContent } from '../../utils/general';
import { Field } from '../../types';
import copySvg from '../../assets/img/copy-yellow.svg';

const options = { month: 'long', day: 'numeric', year: 'numeric', hour: 'numeric', minute: 'numeric', timeZoneName: 'short' };

interface Props {
  entity: string;
  kind?: string;
  item: any;
  changeLevel?(level: number): void;
  goToDetail?(id: string): void;
}


const ItemDisplay: React.FC<Props> = (props) => {
  const { entity, kind, item, changeLevel, goToDetail } = props;
  const fields = getFields(entity, kind);
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
                  <LevelBtn onClick={() => changeLevel(item[field.name] - 1)}>{'<'}</LevelBtn>
                  <LevelLabel>Level</LevelLabel>
                  <LevelBtn onClick={() => changeLevel(item[field.name] + 1)}>{'>'}</LevelBtn>
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
                <FieldContent>{new Intl.DateTimeFormat('default', options).format(item[field.name])}</FieldContent>
              </RowContainer>
            );
          }

          if (field.isLink) {
            return (
              <RowContainer key={field.name} isBottom={index === fields.length - 1}>
                <FieldLabel isBar={index === 0 || index === fields.length - 1}>{field.displayName}</FieldLabel>
                <LinkContent onClick={() => goToDetail(item[field.name])}>{item[field.name]}</LinkContent>
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