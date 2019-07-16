import React from 'react';
import { match } from 'react-router-dom';
import Block from '../Block';
import Account from '../Account';
import Operation from '../Operation';
import { MatchParams } from '../../types';

interface Props {
  match?: match<MatchParams>;
  goToDetail: (id: string) => void;
  goToBlock: (level: number) => void;
  initData: (params: MatchParams) => void;
}

class RouterWrapper extends React.Component<Props, {}> {
  componentDidMount() {
    const { initData, match } = this.props;
    initData(match.params);
  }
  render() {
    const { goToDetail, goToBlock, match } = this.props;
    const { entity } = match.params;
    if (entity === 'operations') {
      return (<Operation  goToDetail={goToDetail} />);
    } else if (entity === 'accounts') {
      return (<Account goToDetail={goToDetail} />);
    } else {
      return (<Block  goToDetail={goToDetail} goToBlock={goToBlock} />);
    }
  }
}

export default RouterWrapper;