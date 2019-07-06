import React from 'react';
import {render} from 'react-dom';
import App from './containers/App';
import * as serviceWorker from './serviceWorker';

const rootEl = document.getElementById('root');

const theme = {

}

render(
    <App/>,
    rootEl
);
serviceWorker.unregister();
