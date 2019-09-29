#ifndef __PASSOA_PLUGIN_HPP__
#define __PASSOA_PLUGIN_HPP__

class FnPtr {
public:
	void wrap() {
		run();
		delete this;
	}
    virtual void run() = 0;
	virtual ~FnPtr() {}
};

template<typename T>
class binder :public FnPtr {
public:
    binder(T x) :inst(x) {
    }
    virtual void run() {
        inst();
    }
    virtual ~binder() {}
private:
    T inst;
};


#define PA_VARARGS -1
typedef void pa_context;
typedef void(*pa_io_push)(FnPtr* );
typedef void(*pa_vp)(void*);
typedef void*(*pa_p)();
typedef void(*pa_vfp)(pa_vp ,void*);
typedef void(*pa_vpi)(pa_context*, int);
typedef void(*pa_vppi)(pa_context*, void*, int);
typedef void(*pa_vpd)(pa_context*, double);
typedef const char* (*pa_sps)(pa_context*, const char*);
typedef void(*pa_vpp)(pa_context*, void*);
typedef int(*pa_ip)(pa_context*);
typedef int(*pa_ips)(pa_context*, const char*);
typedef void(*pa_vp)(pa_context*);
typedef void(*pa_vps)(pa_context*, const char*);
typedef int(*pa_bpis)(pa_context*, int, const char*);
typedef int(*pa_bpii)(pa_context*, int, int);
typedef int(*pa_ipi)(pa_context*, int);
typedef double(*pa_dpi)(pa_context*, int);
typedef const char* (*pa_spi)(pa_context*, int);
typedef void* (*pa_ppi)(pa_context*, int);
typedef void* (*pa_ppi3i)(pa_context*, int, int*);
typedef bool(*pa_bpi)(pa_context*, int);
typedef bool(*pa_vpu)(pa_context*, unsigned int);
typedef const char* (*pa_spisi)(pa_context*, int ,char* ,unsigned int);

typedef int(*pa_c_function)(pa_context *ctx);


struct pa_function_list_entry {
    const char *key;
    pa_c_function value;
    int nargs;
};
typedef int(*pa_put_function_list)(pa_context*, int, const pa_function_list_entry*);
typedef int(*pa_loop_step)(void);
typedef int(*pa_push_loop)(pa_loop_step );
struct pa_plugin {
    pa_context* ctx;
    pa_put_function_list put_function_list;
    pa_vpi push_int;
    pa_vpd push_number;
    pa_sps push_string;
    pa_vpp push_pointer;
    pa_vp push_undefined;
    pa_ip push_object;
    pa_ip push_array;
    pa_ip get_top;
    pa_ppi get_pointer;
    pa_ipi get_int;
    pa_dpi get_number;
    pa_spi get_string;
    pa_bpi is_pointer;
    pa_bpi is_number;
    pa_bpi is_string;
    pa_bpii put_prop_index;
    pa_bpis put_prop_string;
    pa_ips eval_string;
    pa_vpi dup;
    pa_ipi call;
    pa_vp pop;
    pa_ipi get_type;
    pa_bpi is_buffer_data;
    pa_ppi3i get_buffer_data;
    pa_ips error;
    pa_vppi push_external_buffer;
    pa_ppi push_dynamic_buffer;
    pa_ppi push_fixed_buffer;
    pa_vp push_null;
    pa_vpu push_uint;
    pa_io_push post_io;
    pa_bpi is_function;
    pa_p keep_io;
    pa_vp release_io;
    pa_bpi is_array;
    pa_bpii get_prop_index;
    pa_bpis get_prop_string;
    pa_ipi get_length;
    pa_bpi is_boolean;
    pa_bpi get_boolean;
    pa_spisi to_locale_string;
    pa_sps push_utf8_string;
};

template<typename T>
void bind_io(pa_plugin& g, T x) {
    g.post_io(new binder<T>(x));
}

#endif // !__PASSOA_PLUGIN_HPP__

