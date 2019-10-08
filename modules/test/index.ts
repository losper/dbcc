import { dbc_convert, dbc_bind } from '../src/index';
dbc_bind((target, step, total) => {
	console.log(target, step, total);
});
dbc_convert('json', '../ex2.dbc', __dirname);
