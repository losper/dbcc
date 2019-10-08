if ('win32' == process.platform) {
	process.env.Path = process.env.Path + ';' + __dirname;
} else {
	process.env.Path = process.env.Path;
}
const dbcc = require('./dbcc.passoa');
declare let passoa_callbacks: any;

export function bind(cb: (target: string, step: number, total: number) => void) {
	let ref = -1;
	if ('function' == typeof cb) {
		ref = passoa_callbacks.push(cb);
	}
	console.log(dbcc);
	return dbcc.dbcBind(ref);
}
export function dbc2Json(src: string, dst: string, useTimeStamps: boolean) {
	return dbcc.dbc2Json(src, dst, useTimeStamps);
}
export function dbc2C(dbc_path: string, json_dir: string, fname: string) {
	return dbcc.dbc2C(dbc_path, json_dir, fname);
}
