#ifndef _PYCAIRO_PRIVATE_H_
#define _PYCAIRO_PRIVATE_H_

#ifdef _PYCAIRO_H_
#  error "don't include pycairo.h and pycairo-private.h together"
#endif

#define _INSIDE_PYCAIRO_
#include "pycairo.h"


extern PyTypeObject PyCairoMatrix_Type;
extern PyTypeObject PyCairoContext_Type;
extern PyTypeObject PyCairoSurface_Type;
extern PyTypeObject PyCairoPattern_Type;
extern PyTypeObject PyCairoFont_Type;

int       pycairo_check_status(cairo_status_t status);

/* takes ownership of reference */
PyObject *pycairo_matrix_new(cairo_matrix_t *matrix);
PyObject *pycairo_context_wrap(cairo_t *ctx);
PyObject *pycairo_surface_new(cairo_surface_t *surface);
PyObject *pycairo_pattern_wrap(cairo_pattern_t *pattern);
PyObject *pycairo_font_new(cairo_font_t *font);

#endif
