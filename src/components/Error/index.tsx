import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
  position: fixed;
  display: flex;
  align-items: center;
  justify-content: center;
  background-color: rgba(0, 0, 0, 0.3);
  top: 0;
  left: 0;
  width: 100%;
  height: 100vh;
  z-index: 100;
`;
const MainContainer = styled.div`
  width: 445px;
  height: 240px;
  padding: 15px;
  background-color: #c22c2c;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 20px 20px ${props => props.theme.fieldBgColor};
  text-align: center;
`;

const BorderContainer = styled.div`
  width: 100%;
  height: 100%;
  padding: 4px;
  border: 2px solid #ffffff;
`;

const ContentContainer = styled.div`
  width: 100%;
  height: 100%;
  border: 2px solid #ffffff;
`;

const Title = styled.div`
  position: relative;
  font-size: 20px;
  color: #ffffff;
  letter-spacing: 2px;
  display: inline-block;
  padding: 0 15px;
  top: -13px;
  background-color: #c22c2c;
`;

const ErrTxt = styled.p`
  font-size: 20px;
  margin: 35px 0 40px 0;
`;

const OkBtn = styled.button`
  font-size: 20px;
  padding: 7px 15px;
  min-width: 100px;
  background-color: #EEFF00;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 12px 10px ${props => props.theme.fieldBgColor};
  text-align: center;
  letter-spacing: 1px;
  font-family: 'Perfect DOS VGA 437 Win', sans-serif;
  outline: none;
`;

interface Props {
  error: string;
  entity?: string;
  onTry(): void;
  onCancel(): void;
}
const Error: React.FC<Props> = props => {
  const { error, entity, onCancel, onTry } = props;
  const btnTitle = entity === 'operations' ? 'Try Again' : 'Ok';
  function stop(event: any) {
    event.stopPropagation();
  }
  function confirm(event: any) {
    if (entity === 'operations') {
      onTry();
    } else {
      onCancel();
    }
    event.stopPropagation();
  }
  return (
    <Container onClick={onCancel}>
      <MainContainer onClick={stop}>
        <BorderContainer>
          <ContentContainer>
            <Title>! ERROR !</Title>
            <ErrTxt>{error}</ErrTxt>
            <OkBtn onClick={confirm}>{btnTitle}</OkBtn>
          </ContentContainer>
        </BorderContainer>
      </MainContainer>
    </Container>
  );
};

Error.defaultProps = {
  entity: 'blocks'
};

export default Error;