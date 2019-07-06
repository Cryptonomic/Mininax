import React from 'react';
import styled from 'styled-components';
import loadingSvg from '../../assets/img/loading.svg';

const LoadingContainer = styled.div`
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

const LoaderImg = styled.img`

`;

const Loader = () => (
  <LoadingContainer>
    <LoaderImg src={loadingSvg} />
  </LoadingContainer>
);

export default Loader;