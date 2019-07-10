
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
  width: 215px;
  cursor: pointer;
  margin-top: 10px;
  ${TitleLinkCss}
`;

export const SpanIcon = styled.span`
  font-size: 13px;
  vertical-align: middle;
  display: inline-block;
  margin-top: -5px;
`;

const BtnCss = css`
  width: 31px;
  height: 31px;
  font-size: 20px;
  border: none;
  outline: none;
`;

export const TransactionBtn =  styled.button<{disabled?: boolean}>`
  ${BtnCss};
  color: #000000;
  background-color: #EEFF00;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 7px 7px  ${props => props.theme.fieldBgColor};
  margin-right: 7px;
  ${props => props.disabled && css`opacity: 0.7;`};
`;


