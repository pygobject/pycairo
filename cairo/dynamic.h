#ifndef _PYCAIRO_DYNAMIC_H_
#define _PYCAIRO_DYNAMIC_H_

#include <cairo.h>

void* _Pycairo_get_cairo_symbol(const char* symbol_name);

#define _PYCAIRO_DECLARE_CAIRO_FUNC(name, ret_type, params) \
    struct { \
        const char* symbol_name; \
        ret_type (*ptr)params; \
        int checked; \
    } name

typedef struct {
    _PYCAIRO_DECLARE_CAIRO_FUNC(cairo_version, int, (void));
} _Pycairo_cairo_funcs_t;

extern _Pycairo_cairo_funcs_t _Pycairo_cairo_funcs;

#ifdef CAIRO_WIN32_STATIC_BUILD

#define PYCAIRO_GET_CAIRO_FUNC(field) (field)

#define RETURN_NULL_IF_NOT_HAS_CAIRO_FUNC(field)

#define PYCAIRO_HAS_CAIRO_FUNC(field) (1)

#else

#define PYCAIRO_GET_CAIRO_FUNC(field) \
    (_Pycairo_cairo_funcs.field.checked ? _Pycairo_cairo_funcs.field.ptr : \
     (_Pycairo_cairo_funcs.field.checked = 1, \
      _Pycairo_cairo_funcs.field.ptr = _Pycairo_get_cairo_symbol(_Pycairo_cairo_funcs.field.symbol_name)))

#define PYCAIRO_HAS_CAIRO_FUNC(field) \
    (PYCAIRO_GET_CAIRO_FUNC(field) != NULL)

#define RETURN_NULL_IF_NOT_HAS_CAIRO_FUNC(field) \
    do { \
        if (!PYCAIRO_GET_CAIRO_FUNC(field)) { \
            PyErr_Format(PyExc_RuntimeError, "%s not available in this cairo build", \
                         _Pycairo_cairo_funcs.field.symbol_name); \
            return NULL; \
        } \
    } while(0)

#endif

#endif // _PYCAIRO_DYNAMIC_H_
