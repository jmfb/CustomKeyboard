const path = require('path');

module.exports = {
	entry: './src/index',
	mode: 'production',
	output: {
		filename: 'index.js',
		path: path.resolve(__dirname, 'dist')
	},
	resolve: {
		extensions: ['.tsx', '.ts', '.jsx', '.js']
	},
	module: {
		rules: [
			{
				test: /\.(j|t)sx?$/,
				exclude: /node_modules/,
				loader: 'babel-loader'
			}
		]
	}
};
