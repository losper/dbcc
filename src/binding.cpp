#include "binding.h"
#include "notify.h"
#include "../2json.h"
#include "../2c.h"
#include "../util.h"
#include "../parse.h"
#include <iostream>
#include <thread>

extern struct pa_plugin gp;
int gref = -1;
void onCall(std::string fn, size_t pos, size_t length) {
    if ((gref!=-1) && gp.ctx) {
        gp.eval_string(gp.ctx, "(passoa_callbacks.call.bind(passoa_callbacks))");
        gp.push_int(gp.ctx, gref);
        gp.push_string(gp.ctx, fn.c_str());
        gp.push_int(gp.ctx, pos);
        gp.push_int(gp.ctx, length);
        gp.call(gp.ctx, 4);
        gp.pop(gp.ctx);
    }
}
int dbcBind(pa_context *ctx) {
    if (gp.is_number(ctx, 0)) {
        gref = gp.get_int(ctx,0);
    }
    return 0;
}
void dbcnotify(char* fn, size_t pos,size_t length) {
    if (pos<length) {
        if (pos % 10000)return;
    }
    bind_io(gp, std::bind(onCall, std::string(fn), pos, length));
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
void dbc2json_thread(void* p,std::string src, std::string dst, bool use_time_stamps) {
    
    mpc_ast_t* ast = parse_dbc_file_by_name(src.c_str());    
    if(ast){
        dbc_t *dbc = ast2dbc(ast);
        dbc2jsonWrapper(dbc, dst.c_str(), use_time_stamps);
        dbc_delete(dbc);
        mpc_ast_delete(ast);
        bind_io(gp, std::bind(onCall, src, 0, 0));
    }
    else {
        bind_io(gp, std::bind(onCall, src, -1, -1));
    }
    gp.release_io(p);
}
int dbc2Json(pa_context *ctx) {
    void* p = gp.keep_io();
    if (gref == -1)return 0;
    if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) && gp.is_boolean(ctx,2)) {
        std::thread t1(dbc2json_thread,p, gp.get_string(ctx, 0), gp.get_string(ctx, 1),
            gp.get_boolean(ctx, 2));
        t1.detach();
    }
    return 0;
}
void dbc2c_thread(void* p,std::string src,std::string dst,std::string fname) {
    dbc2c_options_t copts = {
        false,true,false,true,true,false
    };
    
    mpc_ast_t* ast = parse_dbc_file_by_name(src.c_str());
    if (ast) {
        dbc_t *dbc = ast2dbc(ast);
        dbc2cWrapper(dbc, dst.c_str(), fname.c_str(), &copts);
        dbc_delete(dbc);
        mpc_ast_delete(ast);
        bind_io(gp, std::bind(onCall, src, 0, 0));
    }
    else {
        bind_io(gp, std::bind(onCall, src, -1, -1));
    }
    gp.release_io(p);
}
int dbc2C(pa_context *ctx) {
    if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) 
        && gp.is_string(ctx, 2)) {
        void* p = gp.keep_io();
        std::thread t1(dbc2c_thread,p,gp.get_string(ctx,0), gp.get_string(ctx, 1),
            gp.get_string(ctx, 2));
        t1.detach();
    }
    return 0;
}