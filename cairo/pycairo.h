/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifndef _PYCAIRO_H_
#define _PYCAIRO_H_

#include <Python.h>
#include <cairo.h>

typedef struct {
    PyObject_HEAD
    cairo_matrix_t *matrix;
} PyCairoMatrix;

typedef struct {
    PyObject_HEAD
    cairo_t *ctx;
} PyCairoContext;

typedef struct {
    PyObject_HEAD
    cairo_surface_t *surface;
} PyCairoSurface;

typedef struct {
    PyObject_HEAD
    cairo_pattern_t *pattern;
} PyCairoPattern;

typedef struct {
    PyObject_HEAD
    cairo_font_t *font;
} PyCairoFont;

struct _PyCairo_FunctionStruct {
    int (* check_status)(cairo_status_t status);
    PyTypeObject *matrix_type;
    PyObject *(* matrix_new)(cairo_matrix_t *matrix);
    PyTypeObject *surface_type;
    PyObject *(* surface_new)(cairo_surface_t *surface);
    PyTypeObject *font_type;
    PyObject *(* font_new)(cairo_font_t *font);
    PyTypeObject *context_type;
    PyObject *(* context_wrap)(cairo_t *ctx);
    PyTypeObject *pattern_type;
    PyObject *(* pattern_wrap)(cairo_pattern_t *pattern);
};

#ifndef _INSIDE_PYCAIRO_

#if defined(NO_IMPORT) || defined(NO_IMPORT_PYCAIRO)
extern struct _PyCairo_FunctionStruct *_PyCairo_API;
#else
struct _PyCairo_FunctionStruct *_PyCairo_API;
#endif

#define pycairo_check_status (_PyCairo_API->check_status)
#define PyCairoMatrix_Type  *(_PyCairo_API->matrix_type)
#define pycairo_matrix_new   (_PyCairo_API->matrix_new)
#define PyCairoSurface_Type *(_PyCairo_API->surface_type)
#define pycairo_surface_new  (_PyCairo_API->surface_new)
#define PyCairoFont_Type    *(_PyCairo_API->font_type)
#define pycairo_font_new     (_PyCairo_API->font_new)
#define PyCairoContext_Type *(_PyCairo_API->context_type)
#define pycairo_context_wrap (_PyCairo_API->context_wrap)
#define PyCairoPattern_Type *(_PyCairo_API->pattern_type)
#define pycairo_pattern_wrap (_PyCairo_API->pattern_wrap)

#define init_pycairo() { \
    PyObject *pycairo = PyImport_ImportModule("cairo._cairo"); \
    if (pycairo != NULL) { \
        PyObject *module_dict = PyModule_GetDict(pycairo); \
        PyObject *cobject = PyDict_GetItemString(module_dict,"_PyCairo_API"); \
        if (PyCObject_Check(cobject)) \
            _PyCairo_API = (struct _PyCairo_FunctionStruct *) \
                PyCObject_AsVoidPtr(cobject); \
        else { \
            PyErr_SetString(PyExc_RuntimeError, \
                            "could not find _PyCairo_API object"); \
            return; \
        } \
    } else { \
        return; \
    } \
}

#endif

#endif
