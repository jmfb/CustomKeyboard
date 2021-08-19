const path = require('path');

module.exports = {
	entry: './src/index',
	output: {
		filename: 'index.js',
		path: path.resolve(__dirname, 'dist')
	},
	module: {
		rules: [
			{
				test: /\.m?(j|t)sx?$/,
				exclude: /node_modules/,
				loader: 'babel-loader'
			}
		]
	}
};
