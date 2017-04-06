/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2003-2005 James Henstridge
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 */

#include <Python.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo-private.h"


/* A module specific exception */
static PyObject *CairoError = NULL;

int
Pycairo_Check_Status (cairo_status_t status)
{
    /* copy strings from cairo.c cairo_status_string() */
    switch (status) {
    case CAIRO_STATUS_SUCCESS:
	return 0;
    case CAIRO_STATUS_NO_MEMORY:
	PyErr_NoMemory();
	return 1;
    case CAIRO_STATUS_INVALID_RESTORE:
	PyErr_SetString(CairoError, "Context.restore without matching "
			"Context.save");
	return 1;
    case CAIRO_STATUS_INVALID_POP_GROUP:
	PyErr_SetString(CairoError, "Context.pop_group without matching "
			"Context.push_group");
	return 1;
    case CAIRO_STATUS_NO_CURRENT_POINT:
	PyErr_SetString(CairoError, "no current point defined");
	return 1;
    case CAIRO_STATUS_INVALID_MATRIX:
	PyErr_SetString(CairoError, "invalid matrix (not invertible)");
	return 1;
    case CAIRO_STATUS_NO_TARGET_SURFACE:
	PyErr_SetString(CairoError, "no target surface has been set");
	return 1;
    case CAIRO_STATUS_NULL_POINTER:
	PyErr_SetString(CairoError, "NULL pointer");
	return 1;
    case CAIRO_STATUS_INVALID_STRING:
	PyErr_SetString(CairoError, "input string not valid UTF-8");
	return 1;
    case CAIRO_STATUS_INVALID_PATH_DATA:
	PyErr_SetString(CairoError, "invalid path data not valid");
	return 1;
    case CAIRO_STATUS_READ_ERROR:
	PyErr_SetString(CairoError, "error while reading from input stream");
	return 1;
    case CAIRO_STATUS_WRITE_ERROR:
	PyErr_SetString(CairoError, "error while writing to output stream");
	return 1;
    case CAIRO_STATUS_SURFACE_FINISHED:
	PyErr_SetString(CairoError, "the target surface has been finished");
	return 1;
    case CAIRO_STATUS_SURFACE_TYPE_MISMATCH:
	PyErr_SetString(CairoError, "the surface type is not appropriate for "
			"the operation");
	return 1;
    case CAIRO_STATUS_BAD_NESTING:
	PyErr_SetString(CairoError, "drawing operations interleaved for two "
			"contexts for the same surface");
	return 1;
    default:
	PyErr_SetString(CairoError, "<unknown error status>");
	return 1;
    }
}


/* C API.  Clients get at this via Pycairo_IMPORT, defined in pycairo.h.
 */
static Pycairo_CAPI_t CAPI = {
    &PycairoContext_Type,  
    &PycairoFontFace_Type, 
    &PycairoMatrix_Type,   
    &PycairoPath_Type,  
    &PycairoPattern_Type,  
    &PycairoScaledFont_Type,  

    &PycairoSurface_Type,  
    &PycairoImageSurface_Type,  
    &PycairoPDFSurface_Type,  
    &PycairoPSSurface_Type,  

    PycairoContext_FromContext,
    PycairoFontFace_FromFontFace,
    PycairoMatrix_FromMatrix,
    PycairoPath_FromPath,
    PycairoPattern_FromPattern,
    PycairoScaledFont_FromScaledFont,

    PycairoSurface_FromSurface,
    PycairoImageSurface_FromImageSurface,
    PycairoPDFSurface_FromPDFSurface,
    PycairoPSSurface_FromPSSurface,

    Pycairo_Check_Status,
};

DL_EXPORT(void)
init_cairo(void)
{
    PyObject *m;

    if (PyType_Ready(&PycairoContext_Type) < 0)
        return;
    if (PyType_Ready(&PycairoFontFace_Type) < 0)
        return;
    if (PyType_Ready(&PycairoMatrix_Type) < 0)
        return;
    if (PyType_Ready(&PycairoPath_Type) < 0)
	return;
    if (PyType_Ready(&PycairoPattern_Type) < 0)
        return;
    if (PyType_Ready(&PycairoScaledFont_Type) < 0)
        return;

    if (PyType_Ready(&PycairoSurface_Type) < 0)
        return;
    if (PyType_Ready(&PycairoImageSurface_Type) < 0)
        return;
    if (PyType_Ready(&PycairoPDFSurface_Type) < 0)
        return;
    if (PyType_Ready(&PycairoPSSurface_Type) < 0)
        return;

    m = Py_InitModule("cairo._cairo", NULL);

    Py_INCREF(&PycairoContext_Type);
    PyModule_AddObject(m, "Context", (PyObject *)&PycairoContext_Type);
    Py_INCREF(&PycairoFontFace_Type);
    PyModule_AddObject(m, "FontFace",(PyObject *)&PycairoFontFace_Type);
    Py_INCREF(&PycairoMatrix_Type);
    PyModule_AddObject(m, "Matrix",  (PyObject *)&PycairoMatrix_Type);
    Py_INCREF(&PycairoPath_Type);
    /* Don't add Path object since it is not accessed directly as 'cairo.Path'
     * PyModule_AddObject(m, "Path", (PyObject *)&PycairoPath_Type);
     */
    Py_INCREF(&PycairoPattern_Type);
    PyModule_AddObject(m, "Pattern", (PyObject *)&PycairoPattern_Type);
    Py_INCREF(&PycairoScaledFont_Type);
    PyModule_AddObject(m, "ScaledFont", (PyObject *)&PycairoScaledFont_Type);

    Py_INCREF(&PycairoSurface_Type);
    PyModule_AddObject(m, "Surface", (PyObject *)&PycairoSurface_Type);
    Py_INCREF(&PycairoImageSurface_Type);
    PyModule_AddObject(m, "ImageSurface", 
		       (PyObject *)&PycairoImageSurface_Type);
    Py_INCREF(&PycairoPDFSurface_Type);
    PyModule_AddObject(m, "PDFSurface", (PyObject *)&PycairoPDFSurface_Type);
    Py_INCREF(&PycairoPSSurface_Type);
    PyModule_AddObject(m, "PSSurface", (PyObject *)&PycairoPSSurface_Type);

    PyModule_AddObject(m, "CAPI", PyCObject_FromVoidPtr(&CAPI, NULL));

    /* Add 'cairo.Error' to the module */
    if (CairoError == NULL) {
	CairoError = PyErr_NewException("cairo.Error", NULL, NULL);
	if (CairoError == NULL)
	    return;
    }
    Py_INCREF(CairoError);
    if (PyModule_AddObject(m, "Error", CairoError) < 0)
	return;

    /* constants */
#define CONSTANT(x) PyModule_AddIntConstant(m, #x, CAIRO_##x)
    CONSTANT(FORMAT_ARGB32);
    CONSTANT(FORMAT_RGB24);
    CONSTANT(FORMAT_A8);
    CONSTANT(FORMAT_A1);

    CONSTANT(OPERATOR_CLEAR);

    CONSTANT(OPERATOR_SOURCE);
    CONSTANT(OPERATOR_OVER);
    CONSTANT(OPERATOR_IN);
    CONSTANT(OPERATOR_OUT);
    CONSTANT(OPERATOR_ATOP);

    CONSTANT(OPERATOR_DEST);
    CONSTANT(OPERATOR_DEST_OVER);
    CONSTANT(OPERATOR_DEST_IN);
    CONSTANT(OPERATOR_DEST_OUT);
    CONSTANT(OPERATOR_DEST_ATOP);

    CONSTANT(OPERATOR_XOR);
    CONSTANT(OPERATOR_ADD);
    CONSTANT(OPERATOR_SATURATE);

    CONSTANT(FILL_RULE_WINDING);
    CONSTANT(FILL_RULE_EVEN_ODD);

    CONSTANT(LINE_CAP_BUTT);
    CONSTANT(LINE_CAP_ROUND);
    CONSTANT(LINE_CAP_SQUARE);

    CONSTANT(LINE_JOIN_MITER);
    CONSTANT(LINE_JOIN_ROUND);
    CONSTANT(LINE_JOIN_BEVEL);

    CONSTANT(FILTER_FAST);
    CONSTANT(FILTER_GOOD);
    CONSTANT(FILTER_BEST);
    CONSTANT(FILTER_NEAREST);
    CONSTANT(FILTER_BILINEAR);
    CONSTANT(FILTER_GAUSSIAN);

    CONSTANT(FONT_WEIGHT_NORMAL);
    CONSTANT(FONT_WEIGHT_BOLD);

    CONSTANT(FONT_SLANT_NORMAL);
    CONSTANT(FONT_SLANT_ITALIC);
    CONSTANT(FONT_SLANT_OBLIQUE);

    CONSTANT(EXTEND_NONE);
    CONSTANT(EXTEND_REPEAT);
    CONSTANT(EXTEND_REFLECT);

    CONSTANT(PATH_MOVE_TO);
    CONSTANT(PATH_LINE_TO);
    CONSTANT(PATH_CURVE_TO);
    CONSTANT(PATH_CLOSE_PATH);
#undef CONSTANT
}
