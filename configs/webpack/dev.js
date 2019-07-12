// development config
const merge = require('webpack-merge');
const webpack = require('webpack');
const commonConfig = require('./common');
const {resolve} = require('path');

module.exports = merge(commonConfig, {
  mode: 'development',
  entry: [
    'webpack-dev-server/client?http://localhost:8200',// bundle the client for webpack-dev-server and connect to the provided endpoint
    './index.tsx' // the entry point of our app
  ],
  output: {
    path: resolve(__dirname, '../../dist'),
    publicPath: '/'
  },
  devServer: {
    port: 8200,
    historyApiFallback: true
  },
  devtool: 'inline-source-map',
  plugins: [
    new webpack.NamedModulesPlugin(), // prints more readable module names in the browser console on HMR updates
  ],
});
