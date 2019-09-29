#include "cmdline.hxx"
#include "2json.h"
#include "util.h"
#include "parse.h"

int dbc2jsonWrapper(dbc_t *dbc, const char *dbc_file, bool use_time_stamps)
{
    FILE *o = fopen_or_die(dbc_file, "wb");
    const int r = dbc2json(dbc, o, use_time_stamps);
    fclose(o);
    return r;
}
int main( int argc, char **argv ) {
    cmdline::parser args;

    args.add<std::string>( "src", 's', "src path", true, "./" );
    args.add<std::string>( "dst", 'd', "dst path", true, "./" );
    args.parse_check( argc, argv );
    mpc_ast_t* ast=parse_dbc_file_by_name(args.get<std::string>("src").c_str());
    dbc_t *dbc = ast2dbc(ast);
    dbc2jsonWrapper(dbc, args.get<std::string>("dst").c_str(),false);
    dbc_delete(dbc);
    mpc_ast_delete(ast);
    return 0;
}
