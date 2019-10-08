import { dbc2C, dbc2Json, bind } from './binding';
import { normalize, basename, extname } from 'path';
import { statSync } from 'fs';
export function dbc_convert(cast: string, src: string, dst: string): void;
export function dbc_convert(cast: string, src: string, dst: string, fname: string): void;
export function dbc_convert(cast: string, src: string, dst: string, fname?: any) {
	if (!statSync(dst).isDirectory()) {
		return;
	}
	dst = normalize(dst + '/');
	switch (typeof fname) {
		case 'string':
			break;
		default:
			fname = basename(src, extname(src));
			break;
	}
	switch (cast) {
		case 'c':
			dbc2C(src, dst, fname);
			break;
		case 'json':
			console.log(dst + fname + '.json');
			dbc2Json(src, dst + fname + '.json', false);
			break;
	}
}
export function dbc_bind(cb: (target: string, step: number, total: number) => void) {
	return bind(cb);
}
