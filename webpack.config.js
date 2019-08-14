const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');
const outputDir = path.join(__dirname, 'build/');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  context: path.join(__dirname, 'src'),
  entry: './Index.bs.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    filename: 'Index.js',
    publicPath: '/'
  },
  module: {
    rules: [
      {
        test: /\.svg(\?v=\d+\.\d+\.\d+)?$/,
        use: {
          loader: 'url-loader',
          options: {
            limit: 10000,
            mimetype: 'image/svg+xml'
          }
        }
      },
      {
        test: /\.css$/,
        use: [{ loader: "style-loader" }, { loader: "css-loader" }]
      }
    ]
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'index.html',
      inject: false
    }),
    new CopyPlugin([
      { from: 'assets', to: 'assets' }
    ])
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: 8400,
    historyApiFallback: true
  },
  node: {
    // handle "Can't resolve 'fs'" issue
    fs: 'empty',
    child_process: 'empty'
  }
};
