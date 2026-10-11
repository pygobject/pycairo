#include <stddef.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #define _GNU_SOURCE
    #include <dlfcn.h>
#endif

#include "dynamic.h"

#define INIT_CAIRO_FUNC(field, symbol_name) \
    .field = {symbol_name, NULL, 0}

_Pycairo_cairo_funcs_t _Pycairo_cairo_funcs = {
    INIT_CAIRO_FUNC(cairo_version, "cairo_version"),
};

void* _Pycairo_get_cairo_symbol(const char* symbol_name) {
    static void* cairo_handle = NULL;
    static int handle_initialized = 0;

    if (!handle_initialized) {
        handle_initialized = 1;
#ifdef _WIN32
        HMODULE handle;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                               GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)cairo_create, &handle)) {
            cairo_handle = handle;
        }
#else
        Dl_info info;
        if (dladdr((void*)cairo_create, &info) != 0) {
            cairo_handle = dlopen(info.dli_fname, RTLD_LAZY | RTLD_NOLOAD);
        }
#endif
    }

    if (!cairo_handle)
        return NULL;

#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)cairo_handle, symbol_name);
#else
    dlerror();
    void* symbol = dlsym(cairo_handle, symbol_name);
    return (dlerror() == NULL) ? symbol : NULL;
#endif
}