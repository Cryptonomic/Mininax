import React from 'react';
import {render} from 'react-dom';
import { HashRouter } from 'react-router-dom';
import { Provider } from 'react-redux';
import { store } from './store';
import App from './containers/App';
import * as serviceWorker from './serviceWorker';

const rootEl = document.getElementById('root');

render(
    <Provider store={store}>
        <HashRouter>
            <App/>
        </HashRouter>
    </Provider>
    ,
    rootEl
);
serviceWorker.unregister();
