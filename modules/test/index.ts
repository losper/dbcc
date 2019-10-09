import { dbc_convert, dbc_bind } from '../src/index';
dbc_bind((target, step, total) => {
	console.log(target, step, total);
	if (step == 0) {
		console.log('parse end!!!');
	} else if (step == -1) {
		console.log('parse fails!!!');
	}
});
dbc_convert('json', '../ex1.dbc', __dirname);
