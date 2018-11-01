/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Pycairo - Python bindings for cairo
 *
 * Copyright © 2017 Christoph Reiter
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

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "private.h"


/* For the CAIRO_SVG_VERSION_* defines */
#ifdef CAIRO_HAS_SVG_SURFACE
#  include <cairo-svg.h>
#endif
/* For the CAIRO_PDF_VERSION_* defines */
#ifdef CAIRO_HAS_PDF_SURFACE
#  include <cairo-pdf.h>
#endif
/* For the CAIRO_PS_LEVEL_* defines */
#ifdef CAIRO_HAS_PS_SURFACE
#  include <cairo-ps.h>
#endif
/* For the CAIRO_SCRIPT_* defines */
#ifdef CAIRO_HAS_SCRIPT_SURFACE
#  include <cairo-script.h>
#endif

typedef struct {
    PYCAIRO_PyLongObject base;
} Pycairo_IntEnumObject;

static PyObject *
int_enum_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  PyObject *dummy;

    if (!PyArg_ParseTuple (args, "O", &dummy))
        return NULL;

    return PYCAIRO_PyLong_Type.tp_new(type, args, kwds);
}

PyObject *
int_enum_create(PyTypeObject *type, long value) {
    PyObject *args, *result;

    args = Py_BuildValue("(l)", value);
    if (args == NULL)
        return NULL;
    result = int_enum_new(type, args, NULL);
    Py_DECREF(args);
    return result;
}

static const char *map_name = "__map";

static PyObject*
enum_type_register_constant(PyTypeObject *type, const char* name, long value) {
    PyObject *value_map;
    PyObject *int_obj, *name_obj, *en;

    /* Get/Create the int->name mapping */
    value_map = PyDict_GetItemString(type->tp_dict, map_name);
    if (value_map == NULL) {
        value_map = PyDict_New();
        PyDict_SetItemString(type->tp_dict, map_name, value_map);
        Py_DECREF(value_map);
    }

    /* Add int->name pair to the mapping */
    int_obj = PYCAIRO_PyLong_FromLong(value);
    name_obj = PYCAIRO_PyUnicode_FromString (name);
    if (PyDict_SetItem(value_map, int_obj, name_obj) < 0) {
        Py_DECREF(int_obj);
        Py_DECREF(name_obj);
        return NULL;
    }
    Py_DECREF(int_obj);
    Py_DECREF(name_obj);

    /* Create a new enum instance of the right type and add to the class */
    en = int_enum_create(type, value);
    if (en == NULL || PyDict_SetItemString(type->tp_dict, name, en) < 0)
        return NULL;

    return en;
}

/* If returns NULL no error is set */
static PyObject *
int_enum_get_name(PyObject *obj) {
    PyObject *value_map, *name_obj;

    value_map = PyDict_GetItemString(Py_TYPE(obj)->tp_dict, map_name);
    if(value_map == NULL)
        return NULL;

    name_obj = PyDict_GetItem(value_map, obj);
    if(name_obj == NULL)
        return NULL;

    return PYCAIRO_PyUnicode_FromFormat("%s.%s", Py_TYPE(obj)->tp_name,
                                        PYCAIRO_PyUnicode_Astring(name_obj));
}

static PyObject *
int_enum_repr(PyObject *obj)
{
    PyObject *name_obj;

    name_obj = int_enum_get_name(obj);
    if(name_obj == NULL)
        return PYCAIRO_PyLong_Type.tp_repr(obj);

    return name_obj;
}

static PyObject *
int_enum_reduce(PyObject *self, PyObject *ignored)
{
    PyObject *num = PYCAIRO_PyNumber_Long (self);
    if (num == NULL)
        return NULL;
    return Py_BuildValue ("(O, (N))", &PYCAIRO_PyLong_Type, num);
}

static PyMethodDef int_enum_methods[] = {
    {"__reduce__", (PyCFunction)int_enum_reduce, METH_NOARGS},
    {NULL, NULL, 0},
};

PyTypeObject Pycairo_IntEnum_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cairo._IntEnum",
    sizeof(Pycairo_IntEnumObject),
};

#define DEFINE_ENUM(n) \
    PyTypeObject Pycairo_##n##_Type = { \
        PyVarObject_HEAD_INIT(NULL, 0) \
        "cairo." #n, \
        sizeof(Pycairo_IntEnumObject), \
    };

DEFINE_ENUM(Antialias)
DEFINE_ENUM(Content)
DEFINE_ENUM(Extend)
DEFINE_ENUM(FillRule)
DEFINE_ENUM(Filter)
DEFINE_ENUM(FontSlant)
DEFINE_ENUM(FontWeight)
DEFINE_ENUM(Format)
DEFINE_ENUM(HintMetrics)
DEFINE_ENUM(HintStyle)
DEFINE_ENUM(LineCap)
DEFINE_ENUM(LineJoin)
DEFINE_ENUM(Operator)
DEFINE_ENUM(PathDataType)
DEFINE_ENUM(RegionOverlap)
DEFINE_ENUM(Status)
DEFINE_ENUM(SubpixelOrder)
DEFINE_ENUM(TextClusterFlags)
DEFINE_ENUM(SurfaceObserverMode)
#ifdef CAIRO_HAS_SVG_SURFACE
DEFINE_ENUM(SVGVersion)
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
DEFINE_ENUM(SVGUnit)
#endif
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
DEFINE_ENUM(PDFVersion)
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
DEFINE_ENUM(PDFMetadata);
DEFINE_ENUM(PDFOutlineFlags);
#endif
#endif
#ifdef CAIRO_HAS_PS_SURFACE
DEFINE_ENUM(PSLevel)
#endif
#ifdef CAIRO_HAS_SCRIPT_SURFACE
DEFINE_ENUM(ScriptMode)
#endif

#undef DEFINE_ENUM

static int
init_enum_type (PyObject *module, const char *name, PyTypeObject *type) {
    type->tp_base = &Pycairo_IntEnum_Type;
    type->tp_flags = Py_TPFLAGS_DEFAULT;

    if (PyType_Ready(type) < 0)
        return -1;

    Py_INCREF(type);
    if (PyModule_AddObject(module, name, (PyObject *)type) < 0)
        return -1;

    return 0;
}

static PyObject *
format_stride_for_width (PyObject *self, PyObject *args) {
  cairo_format_t format;
  long value;
  int width;

  if (!PyArg_ParseTuple(args, "i:stride_for_width", &width))
    return NULL;

  value = PyLong_AsLong (self);
  if (PyErr_Occurred())
    return NULL;
  if (value > INT_MAX || value < INT_MIN) {
    PyErr_SetString (PyExc_ValueError, "format value out of range");
    return NULL;
  }

  format = (cairo_format_t)value;

  return PYCAIRO_PyLong_FromLong (
    cairo_format_stride_for_width (format, width));
}

static PyMethodDef format_methods[] = {
  {"stride_for_width", (PyCFunction)format_stride_for_width, METH_VARARGS},
  {NULL, NULL, 0, NULL},
};

int
init_enums (PyObject *module) {
    PyObject *ev;

    Pycairo_IntEnum_Type.tp_repr = (reprfunc)int_enum_repr;
    Pycairo_IntEnum_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
    Pycairo_IntEnum_Type.tp_methods = int_enum_methods;
    Pycairo_IntEnum_Type.tp_base = &PYCAIRO_PyLong_Type;
    Pycairo_IntEnum_Type.tp_new = (newfunc)int_enum_new;

    if (PyType_Ready(&Pycairo_IntEnum_Type) < 0)
        return -1;

#define ENUM(t) \
    if (init_enum_type(module, #t, &Pycairo_##t##_Type) < 0) \
        return -1;

#define CONSTANT(t, a, b) \
    ev = enum_type_register_constant(&Pycairo_##t##_Type, #b, CAIRO_##a##_##b); \
    if (ev == NULL || PyModule_AddObject(module, #a "_" #b, ev) < 0) \
        return -1;

    ENUM(Antialias);
    CONSTANT(Antialias, ANTIALIAS, DEFAULT);
    CONSTANT(Antialias, ANTIALIAS, NONE);
    CONSTANT(Antialias, ANTIALIAS, GRAY);
    CONSTANT(Antialias, ANTIALIAS, SUBPIXEL);
    CONSTANT(Antialias, ANTIALIAS, FAST);
    CONSTANT(Antialias, ANTIALIAS, GOOD);
    CONSTANT(Antialias, ANTIALIAS, BEST);

    ENUM(Content);
    CONSTANT(Content, CONTENT, COLOR);
    CONSTANT(Content, CONTENT, ALPHA);
    CONSTANT(Content, CONTENT, COLOR_ALPHA);

    ENUM(Extend);
    CONSTANT(Extend, EXTEND, NONE);
    CONSTANT(Extend, EXTEND, REPEAT);
    CONSTANT(Extend, EXTEND, REFLECT);
    CONSTANT(Extend, EXTEND, PAD);

    ENUM(FillRule);
    CONSTANT(FillRule, FILL_RULE, WINDING);
    CONSTANT(FillRule, FILL_RULE, EVEN_ODD);

    ENUM(Filter);
    CONSTANT(Filter, FILTER, FAST);
    CONSTANT(Filter, FILTER, GOOD);
    CONSTANT(Filter, FILTER, BEST);
    CONSTANT(Filter, FILTER, NEAREST);
    CONSTANT(Filter, FILTER, BILINEAR);
    CONSTANT(Filter, FILTER, GAUSSIAN);

    ENUM(FontWeight)
    CONSTANT(FontWeight, FONT_WEIGHT, NORMAL);
    CONSTANT(FontWeight, FONT_WEIGHT, BOLD);

    ENUM(FontSlant);
    CONSTANT(FontSlant, FONT_SLANT, NORMAL);
    CONSTANT(FontSlant, FONT_SLANT, ITALIC);
    CONSTANT(FontSlant, FONT_SLANT, OBLIQUE);

    Pycairo_Format_Type.tp_methods = format_methods;
    ENUM(Format);
    CONSTANT(Format, FORMAT, INVALID);
    CONSTANT(Format, FORMAT, ARGB32);
    CONSTANT(Format, FORMAT, RGB24);
    CONSTANT(Format, FORMAT, A8);
    CONSTANT(Format, FORMAT, A1);
    CONSTANT(Format, FORMAT, RGB16_565);
    CONSTANT(Format, FORMAT, RGB30);

    ENUM(HintMetrics);
    CONSTANT(HintMetrics, HINT_METRICS, DEFAULT);
    CONSTANT(HintMetrics, HINT_METRICS, OFF);
    CONSTANT(HintMetrics, HINT_METRICS, ON);

    ENUM(HintStyle);
    CONSTANT(HintStyle, HINT_STYLE, DEFAULT);
    CONSTANT(HintStyle, HINT_STYLE, NONE);
    CONSTANT(HintStyle, HINT_STYLE, SLIGHT);
    CONSTANT(HintStyle, HINT_STYLE, MEDIUM);
    CONSTANT(HintStyle, HINT_STYLE, FULL);

    ENUM(LineCap);
    CONSTANT(LineCap, LINE_CAP, BUTT);
    CONSTANT(LineCap, LINE_CAP, ROUND);
    CONSTANT(LineCap, LINE_CAP, SQUARE);

    ENUM(LineJoin);
    CONSTANT(LineJoin, LINE_JOIN, MITER);
    CONSTANT(LineJoin, LINE_JOIN, ROUND);
    CONSTANT(LineJoin, LINE_JOIN, BEVEL);

    ENUM(Operator);
    CONSTANT(Operator, OPERATOR, CLEAR);
    CONSTANT(Operator, OPERATOR, SOURCE);
    CONSTANT(Operator, OPERATOR, OVER);
    CONSTANT(Operator, OPERATOR, IN);
    CONSTANT(Operator, OPERATOR, OUT);
    CONSTANT(Operator, OPERATOR, ATOP);
    CONSTANT(Operator, OPERATOR, DEST);
    CONSTANT(Operator, OPERATOR, DEST_OVER);
    CONSTANT(Operator, OPERATOR, DEST_IN);
    CONSTANT(Operator, OPERATOR, DEST_OUT);
    CONSTANT(Operator, OPERATOR, DEST_ATOP);
    CONSTANT(Operator, OPERATOR, XOR);
    CONSTANT(Operator, OPERATOR, ADD);
    CONSTANT(Operator, OPERATOR, SATURATE);
    CONSTANT(Operator, OPERATOR, MULTIPLY);
    CONSTANT(Operator, OPERATOR, SCREEN);
    CONSTANT(Operator, OPERATOR, OVERLAY);
    CONSTANT(Operator, OPERATOR, DARKEN);
    CONSTANT(Operator, OPERATOR, LIGHTEN);
    CONSTANT(Operator, OPERATOR, COLOR_DODGE);
    CONSTANT(Operator, OPERATOR, COLOR_BURN);
    CONSTANT(Operator, OPERATOR, HARD_LIGHT);
    CONSTANT(Operator, OPERATOR, SOFT_LIGHT);
    CONSTANT(Operator, OPERATOR, DIFFERENCE);
    CONSTANT(Operator, OPERATOR, EXCLUSION);
    CONSTANT(Operator, OPERATOR, HSL_HUE);
    CONSTANT(Operator, OPERATOR, HSL_SATURATION);
    CONSTANT(Operator, OPERATOR, HSL_COLOR);
    CONSTANT(Operator, OPERATOR, HSL_LUMINOSITY);

    ENUM(Status);
    CONSTANT(Status, STATUS, SUCCESS);
    CONSTANT(Status, STATUS, NO_MEMORY);
    CONSTANT(Status, STATUS, INVALID_RESTORE);
    CONSTANT(Status, STATUS, INVALID_POP_GROUP);
    CONSTANT(Status, STATUS, NO_CURRENT_POINT);
    CONSTANT(Status, STATUS, INVALID_MATRIX);
    CONSTANT(Status, STATUS, INVALID_STATUS);
    CONSTANT(Status, STATUS, NULL_POINTER);
    CONSTANT(Status, STATUS, INVALID_STRING);
    CONSTANT(Status, STATUS, INVALID_PATH_DATA);
    CONSTANT(Status, STATUS, READ_ERROR);
    CONSTANT(Status, STATUS, WRITE_ERROR);
    CONSTANT(Status, STATUS, SURFACE_FINISHED);
    CONSTANT(Status, STATUS, SURFACE_TYPE_MISMATCH);
    CONSTANT(Status, STATUS, PATTERN_TYPE_MISMATCH);
    CONSTANT(Status, STATUS, INVALID_CONTENT);
    CONSTANT(Status, STATUS, INVALID_FORMAT);
    CONSTANT(Status, STATUS, INVALID_VISUAL);
    CONSTANT(Status, STATUS, FILE_NOT_FOUND);
    CONSTANT(Status, STATUS, INVALID_DASH);
    CONSTANT(Status, STATUS, INVALID_DSC_COMMENT);
    CONSTANT(Status, STATUS, INVALID_INDEX);
    CONSTANT(Status, STATUS, CLIP_NOT_REPRESENTABLE);
    CONSTANT(Status, STATUS, TEMP_FILE_ERROR);
    CONSTANT(Status, STATUS, INVALID_STRIDE);
    CONSTANT(Status, STATUS, FONT_TYPE_MISMATCH);
    CONSTANT(Status, STATUS, USER_FONT_IMMUTABLE);
    CONSTANT(Status, STATUS, USER_FONT_ERROR);
    CONSTANT(Status, STATUS, NEGATIVE_COUNT);
    CONSTANT(Status, STATUS, INVALID_CLUSTERS);
    CONSTANT(Status, STATUS, INVALID_SLANT);
    CONSTANT(Status, STATUS, INVALID_WEIGHT);
    CONSTANT(Status, STATUS, INVALID_SIZE);
    CONSTANT(Status, STATUS, USER_FONT_NOT_IMPLEMENTED);
    CONSTANT(Status, STATUS, DEVICE_TYPE_MISMATCH);
    CONSTANT(Status, STATUS, DEVICE_ERROR);
    CONSTANT(Status, STATUS, INVALID_MESH_CONSTRUCTION);
    CONSTANT(Status, STATUS, DEVICE_FINISHED);
    CONSTANT(Status, STATUS, JBIG2_GLOBAL_MISSING);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
    CONSTANT(Status, STATUS, PNG_ERROR);
    CONSTANT(Status, STATUS, FREETYPE_ERROR);
    CONSTANT(Status, STATUS, WIN32_GDI_ERROR);
    CONSTANT(Status, STATUS, TAG_ERROR);
#endif
    CONSTANT(Status, STATUS, LAST_STATUS);

    ENUM(PathDataType);
    CONSTANT(PathDataType, PATH, MOVE_TO);
    CONSTANT(PathDataType, PATH, LINE_TO);
    CONSTANT(PathDataType, PATH, CURVE_TO);
    CONSTANT(PathDataType, PATH, CLOSE_PATH);

    ENUM(RegionOverlap);
    CONSTANT(RegionOverlap, REGION_OVERLAP, IN);
    CONSTANT(RegionOverlap, REGION_OVERLAP, OUT);
    CONSTANT(RegionOverlap, REGION_OVERLAP, PART);

    ENUM(SubpixelOrder);
    CONSTANT(SubpixelOrder, SUBPIXEL_ORDER, DEFAULT);
    CONSTANT(SubpixelOrder, SUBPIXEL_ORDER, RGB);
    CONSTANT(SubpixelOrder, SUBPIXEL_ORDER, BGR);
    CONSTANT(SubpixelOrder, SUBPIXEL_ORDER, VRGB);
    CONSTANT(SubpixelOrder, SUBPIXEL_ORDER, VBGR);

    ENUM(TextClusterFlags);
    CONSTANT(TextClusterFlags, TEXT_CLUSTER_FLAG, BACKWARD);

    ENUM(SurfaceObserverMode);
    CONSTANT(SurfaceObserverMode, SURFACE_OBSERVER, NORMAL);
    CONSTANT(SurfaceObserverMode, SURFACE_OBSERVER, RECORD_OPERATIONS);

#ifdef CAIRO_HAS_SVG_SURFACE
    ENUM(SVGVersion);
    CONSTANT(SVGVersion, SVG, VERSION_1_1);
    CONSTANT(SVGVersion, SVG, VERSION_1_2);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
    ENUM(SVGUnit);
    CONSTANT(SVGUnit, SVG_UNIT, USER);
    CONSTANT(SVGUnit, SVG_UNIT, EM);
    CONSTANT(SVGUnit, SVG_UNIT, EX);
    CONSTANT(SVGUnit, SVG_UNIT, PX);
    CONSTANT(SVGUnit, SVG_UNIT, IN);
    CONSTANT(SVGUnit, SVG_UNIT, CM);
    CONSTANT(SVGUnit, SVG_UNIT, MM);
    CONSTANT(SVGUnit, SVG_UNIT, PT);
    CONSTANT(SVGUnit, SVG_UNIT, PC);
    CONSTANT(SVGUnit, SVG_UNIT, PERCENT);
#endif
#endif

#ifdef CAIRO_HAS_PDF_SURFACE
    ENUM(PDFVersion);
    CONSTANT(PDFVersion, PDF, VERSION_1_4);
    CONSTANT(PDFVersion, PDF, VERSION_1_5);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 15, 10)
    ENUM(PDFMetadata);
    CONSTANT(PDFMetadata, PDF_METADATA, TITLE);
    CONSTANT(PDFMetadata, PDF_METADATA, AUTHOR);
    CONSTANT(PDFMetadata, PDF_METADATA, SUBJECT);
    CONSTANT(PDFMetadata, PDF_METADATA, KEYWORDS);
    CONSTANT(PDFMetadata, PDF_METADATA, CREATOR);
    CONSTANT(PDFMetadata, PDF_METADATA, CREATE_DATE);
    CONSTANT(PDFMetadata, PDF_METADATA, MOD_DATE);
    ENUM(PDFOutlineFlags);
    CONSTANT(PDFOutlineFlags, PDF_OUTLINE_FLAG, OPEN);
    CONSTANT(PDFOutlineFlags, PDF_OUTLINE_FLAG, BOLD);
    CONSTANT(PDFOutlineFlags, PDF_OUTLINE_FLAG, ITALIC);
#endif
#endif

#ifdef CAIRO_HAS_PS_SURFACE
    ENUM(PSLevel);
    CONSTANT(PSLevel, PS, LEVEL_2);
    CONSTANT(PSLevel, PS, LEVEL_3);
#endif

#ifdef CAIRO_HAS_SCRIPT_SURFACE
    ENUM(ScriptMode);
    CONSTANT(ScriptMode, SCRIPT_MODE, ASCII);
    CONSTANT(ScriptMode, SCRIPT_MODE, BINARY);
#endif

#undef ENUM
#undef CONSTANT

    return 0;
}
