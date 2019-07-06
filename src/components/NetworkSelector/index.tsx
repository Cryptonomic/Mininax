import React from 'react';
import styled from 'styled-components';
import configs from '../../config';
import { Config } from '../../types';

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
  width: 400px;
  height: 285px;
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

const SelectContainer = styled.div`
  padding: 30px 48px 10px 48px;
`;

const RowContainer = styled.div`
  font-size: 20px;
  margin-bottom: 35px;
  display: flex;
  align-items: center;
`;

const CheckBtn = styled.button`
  margin-right: 40px;
  width: 20px;
  height: 20px;
  font-family: 'Perfect DOS VGA 437 Win', sans-serif;
  box-shadow: 3px 3px ${props => props.theme.fieldBgColor};
  border: none;
  font-size: 18px;
  outline: none;
`;

interface Props {
  selectedConfig: Config;
  onChange(config: Config): void;
  onCancel(): void;
}
const NetworkSelector: React.FC<Props> = props => {
  const { selectedConfig, onCancel, onChange } = props;
  const selectedIndex = configs.findIndex(config => selectedConfig.network === config.network) || 0;
  const [ selected, setSelected ] = React.useState(selectedIndex);
  function change(event: any, index: number) {
    setSelected(index);
    event.stopPropagation();
  }

  function confirm(event: any) {
    onChange(configs[selected]);
    event.stopPropagation();
  }
  function stop(event: any) {
    event.stopPropagation();
  }
  return (
    <Container onClick={onCancel}>
      <MainContainer onClick={stop}>
        <BorderContainer>
          <ContentContainer>
            <Title>Select a Network</Title>
            <SelectContainer>
              {
                configs.map((config, index) => (
                  <RowContainer key={config.network}>
                    <CheckBtn onClick={(event) => change(event, index)}>{selected === index ? 'x' : ''}</CheckBtn>
                    {config.displayName}
                  </RowContainer>
                ))
              }
            </SelectContainer>
            <OkBtn onClick={confirm}>Ok</OkBtn>
          </ContentContainer>
        </BorderContainer>
      </MainContainer>
    </Container>
  );
};

export default NetworkSelector;