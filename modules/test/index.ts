import { dbc_convert } from '../src/index';
dbc_convert('c', 'ex2.dbc', __dirname, (target, step, total) => {
	console.log(target, step, total);
});
