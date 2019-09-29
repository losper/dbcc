import { dbc2C, dbc2Json } from './binding';
import { normalize, basename, extname } from 'path';
import { statSync } from 'fs';
export function dbc_convert(
	cast: string,
	src: string,
	dst: string,
	cb: (target: string, step: number, total: number) => void
): void;
export function dbc_convert(
	cast: string,
	src: string,
	dst: string,
	fname: string,
	cb: (target: string, step: number, total: number) => void
): void;
export function dbc_convert(cast: string, src: string, dst: string, fname: any, cb?: any) {
	if (!statSync(dst).isDirectory()) {
		return;
	}
	dst = normalize(dst + '/');
	switch (typeof fname) {
		case 'string':
			break;
		case 'function':
			cb = fname;
		default:
			fname = basename(src, extname(src));
			break;
	}
	switch (cast) {
		case 'c':
			dbc2C(src, dst, fname, cb);
			break;
		case 'json':
			dbc2C(src, dst, fname, cb);
			break;
	}
}
