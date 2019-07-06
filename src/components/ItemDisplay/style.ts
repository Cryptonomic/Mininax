
import styled, { css } from 'styled-components';


export const Container = styled.div`
  width: 100%;
  padding-left: 11px;
  position: relative;
  &::before {
    content: '';
    position: absolute;
    width: 2px;
    height: calc(100% - 29px);
    background-color: ${props => props.theme.fieldBgColor};
    left: 0;
    top: 16px;
  }
`;

export const TitleContainer = styled.div`
  display: flex;
  align-items: center;
`;

export const TitleBtnContainer = styled.div`
  margin-left: 30px;
`;

const LeftBarCss = css`
  &::before {
    content: '';
    position: absolute;
    width: 8px;
    height: 2px;
    background-color: ${props => props.theme.fieldBgColor};
    left: -9px;
    top: 16px;
  }
`;

const TitleLinkCss = css`
  position: relative;
  height: 31px;
  padding: 6px 9px;
  font-size: 20px;
  color: #000000;
  background-color: white;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 7px 7px ${props => props.theme.fieldBgColor};
  ${LeftBarCss}
`;

export const Title = styled.div`
  text-transform: capitalize;
  width: 215px;
  ${TitleLinkCss}
`;

export const LinkBtn = styled.div`
  width: 338px;
  ${TitleLinkCss}
`;

export const SpanIcon = styled.span`
  font-size: 13px;
  vertical-align: middle;
  display: inline-block;
  margin-top: -5px;
`;

export const MainContainer = styled.div`
  display: flex;
  flex-direction: column;
  margin-top: 20px;
  margin-bottom: 8px;
`;

export const RowContainer = styled.div<{isBottom?: boolean}>`
  display: flex;
  align-items: center;
  margin-bottom: ${props => props.isBottom ? '0' : '10px'};
`;

export const LevelLabelContainer = styled.div`
  display: flex;
  width: 215px;
  height: 31px;
  justify-content: space-between;
`;

const BtnCss = css`
  width: 31px;
  height: 31px;
  font-size: 20px;
  border: none;
  outline: none;
`;

export const LevelBtn = styled.button`
  ${BtnCss};
  color: ${props => props.theme.fieldColor};
  background-color: ${props => props.theme.fieldBgColor};
`;

export const TransactionBtn =  styled.button<{disabled?: boolean}>`
  ${BtnCss};
  color: #000000;
  background-color: #EEFF00;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 7px 7px  ${props => props.theme.fieldBgColor};
  margin-right: 7px;
  ${props => props.disabled && css`opacity: 0.7;`};
`;

const LabelCss = css`
  position: relative;
  height: 31px;
  padding: 7px 9px 6px 9px;
  font-size: 20px;
  color: ${props => props.theme.fieldColor};
  background-color: ${props => props.theme.fieldBgColor};
`;

export const LevelLabel = styled.div`
  ${LabelCss};
  width: 125px;
  text-align: center;
`;

export const FieldLabel = styled.div<{isBar?: boolean}>`
  ${LabelCss};
  width: 215px;
  ${props => props.isBar && LeftBarCss};
`;

export const FieldContent = styled.div`
  margin-left: 30px;
  color: ${props => props.theme.valueColor};
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
  flex: 1;
`;

export const CopyImg = styled.img`
  float: right;
  cursor: pointer;
`;


