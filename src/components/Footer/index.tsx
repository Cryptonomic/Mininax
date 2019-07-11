import React from 'react';
import styled from 'styled-components';

import cryptoLogo from '../../assets/img/cryptonomic.svg';
import searchSvg from '../../assets/img/search.svg';

const Container = styled.div`
  display: flex;
  padding: 0 55px 45px 45px;
  align-items: center;
`;
const Input = styled.input`
  height: 60px;
  width: 800px;
  border-radius: 10px;
  padding: 20px 24px;
  color: #000000;
  font-size: 18px;
  outline: none;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 0 4px 12px 0 rgba(0,0,0,.12);
  border: 1px solid rgba(0,0,0,.12);
  font-family: 'Perfect DOS VGA 437 Win', sans-serif;
  background-color: ${props => props.theme.searchBgColor};
`;

const SearhBtn = styled.button`
  width: 60px;
  height: 60px;
  margin-left: 15px;
  border-radius: 10px;
  outline: none;
  background-color: ${props => props.theme.searchBgColor};
`;

const CryptoImg = styled.img`
  margin-left: 35px;
  width: 55px;
  height: 55px;
  cursor: pointer;
`;

const ChangeBtn = styled.button`
  text-transform: uppercase;
  margin-left: auto;
  width: 185px;
  height: 31px;
  font-size: 18px;
  color: ${props => props.theme.mainBgColor};
  background-color: white;
  box-shadow: 0 4px 12px 0 rgba(0,0,0,.2), 0 4px 12px 0 rgba(0,0,0,.14), 7px 7px ${props => props.theme.fieldBgColor};
  font-family: 'Perfect DOS VGA 437 Win', sans-serif;
`;

const UpDown = styled.span`
  font-size: 20px;
`;

const SearchImg = styled.img``;

interface Props {
  network: string;
  onOpenNetworkSelector(): void;
  onSearch(val: string): void;
  gotoHome(): void;
}

interface States {
  isFocus: boolean;
  searchVal: string;
}

class Footer extends React.Component<Props, States> {
  textInput: React.RefObject<HTMLInputElement> = React.createRef();
  constructor(props) {
    super(props);
    this.state = {
      isFocus: false,
      searchVal: ''
    };
  }

  onChange = (e) => {
    const searchVal = e.target.value.replace(/ /g, '');
    this.setState({searchVal});
  }

  search = () => {
    const { onSearch } = this.props;
    const { searchVal } = this.state;
    if (searchVal !== '') {
      onSearch(searchVal);
    }
  }

  focusSearch = () => {
    const { isFocus } = this.state;
    if (!isFocus) {
      setTimeout(() => {
        this.textInput.current.focus();
      });
    }
  }

  render() {
    const { network, onOpenNetworkSelector, gotoHome } = this.props;
    const { searchVal } = this.state;

    return (
      <Container>
        <Input
          ref={this.textInput}
          value={searchVal}
          placeholder="Press 's' and search by block ID, operation ID, account ID or block level"
          onChange={this.onChange}
          onBlur={() => this.setState({isFocus: false})}
          onFocus={() => this.setState({isFocus: true})}
          onKeyPress= { e => {
            if (e.key === 'Enter') {
              this.search();
            }
          }}
        />
        <SearhBtn onClick={this.search}>
          <SearchImg src={searchSvg} />
        </SearhBtn>
        <ChangeBtn onClick={onOpenNetworkSelector}>{network} <UpDown>⇵</UpDown></ChangeBtn>
        <CryptoImg src={cryptoLogo} onClick={gotoHome} />
      </Container>
    );
  }
};

export default Footer;
