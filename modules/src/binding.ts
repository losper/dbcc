if ('win32' == process.platform) {
	process.env.Path = process.env.Path + ';' + __dirname;
} else {
	process.env.Path = process.env.Path;
}
const dbcc = require('./dbcc.passoa');
declare let passoa_callbacks: any;

export function dbc2Json(
	dbc_path: string,
	json_dir: string,
	fname: string,
	cb: (target: string, step: number, total: number) => void
) {
	let ref = -1;
	if ('function' == typeof cb) {
		ref = passoa_callbacks.push(cb);
	}
	return dbcc.dbc2Json(dbc_path, json_dir, fname, ref);
}
export function dbc2C(
	dbc_path: string,
	json_dir: string,
	fname: string,
	cb: (target: string, step: number, total: number) => void
) {
	let ref = -1;
	if ('function' == typeof cb) {
		ref = passoa_callbacks.push(cb);
	}
	return dbcc.dbc2C(dbc_path, json_dir, fname, ref);
}
