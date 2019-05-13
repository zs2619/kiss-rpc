var path = require('path');
module.exports = {
	entry: './example/HelloClient.ts',
	devtool: 'inline-source-map',
	output: {
	  filename: 'bundle.js'
	},
	module: {
		rules: [
		{
			test: /\.tsx?$/,
			use: 'ts-loader',
			exclude: /node_modules/
		}
		]
	},
    resolve: {
		modules: [path.resolve(__dirname, "rpc"),"node_modules"],
    	extensions: ['.ts','.js']

    },


	mode: 'development'

  };