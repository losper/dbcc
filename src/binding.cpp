#include "binding.h"
#include "../2json.h"
#include "../2c.h"
#include "../util.h"
#include "../parse.h"
#include <iostream>
#include <thread>

extern struct pa_plugin gp;
int gref = -1;
void onCall(char* fn,int step) {
    if (gp.ctx) {
        gp.eval_string(gp.ctx, "(passoa_callbacks.call.bind(passoa_callbacks))");
        gp.push_int(gp.ctx, gref);
        gp.push_string(gp.ctx, fn);
        gp.push_int(gp.ctx, step);
        //gp.push_int(gp.ctx, total);
        gp.call(gp.ctx, 3);
        gp.pop(gp.ctx);
    }
}
void notify(char* fn, int step) {
    if (!step % 10) {
        onCall(fn, step);
    }
}
int dbc2jsonWrapper(dbc_t *dbc, const char *dbc_file, bool use_time_stamps)
{
    FILE *o = fopen(dbc_file, "wb");
    int r=0;
    if (o) {
        r = dbc2json(dbc, o, use_time_stamps);
        fclose(o);
    }
    return r;
}
int dbc2cWrapper(dbc_t *dbc, std::string dir,std::string fname, dbc2c_options_t *copts)
{
    std::string cname = dir+ fname + ".c";
    std::string hname = dir + fname + ".h";
    FILE *c = fopen(cname.c_str(), "wb");
    FILE *h = fopen(hname.c_str(), "wb");
    if (!c) {
        return -1;
    }
    if (!h) {
        fclose(c);
        return -2;
    }
    const int r = dbc2c(dbc, c, h, fname.c_str(), copts);
    fclose(c);
    fclose(h);
    return r;
}
int dbc2Json(pa_context *ctx) {
    if (gref == -1)return 0;
    if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) && gp.is_number(ctx,2)) {
        mpc_ast_t* ast = parse_dbc_file_by_name(gp.get_string(ctx, 0));
        dbc_t *dbc = ast2dbc(ast);
        dbc2jsonWrapper(dbc, gp.get_string(ctx, 1), false);
        dbc_delete(dbc);
        mpc_ast_delete(ast);
    }
    return 0;
}
void dbc2c_thread(std::string src,std::string dst,std::string fname,int ref) {
    dbc2c_options_t copts = {
        false,true,false,true,true,false
    };
    void* p=gp.keep_io();
    gref = ref;
    mpc_ast_t* ast = parse_dbc_file_by_name(src.c_str());
    dbc_t *dbc = ast2dbc(ast);
    dbc2cWrapper(dbc, dst.c_str(), fname.c_str(), &copts);
    dbc_delete(dbc);
    mpc_ast_delete(ast);
    gref = -1;
    gp.release_io(p);
}
int dbc2C(pa_context *ctx) {
    if (gref == -1)return 0;
    if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) 
        && gp.is_string(ctx, 2) && gp.is_number(ctx, 3)) {
        std::thread t1(dbc2c_thread,gp.get_string(ctx,0), gp.get_string(ctx, 1),
            gp.get_string(ctx, 2), gp.is_number(ctx, 3));
        t1.detach();
    }
    return 0;
}