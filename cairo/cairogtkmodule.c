/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * PyCairo - Python bindings for Cairo
 *
 * Copyright © 2003-2004 James Henstridge
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
 *
 * Contributor(s):
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "pycairo.h"
#include "pycairo-misc.h"
#include <cairo.h>
#include <pygtk/pygtk.h>
#include <gdk/gdkx.h>

static PyTypeObject *_PyGdkDrawable_Type;
#define PyGdkDrawable_Type (*_PyGdkDrawable_Type)
static PyTypeObject *_PyGdkPixbuf_Type;
#define PyGdkPixbuf_Type (*_PyGdkPixbuf_Type)

static PyObject *
surface_create_for_drawable(PyObject *self, PyObject *args)
{
    PyGObject *drawable;
    cairo_format_t format = CAIRO_FORMAT_ARGB32;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!|i:surface_create_for_drawable",
			  &PyGdkDrawable_Type, &drawable, &format))
	return NULL;

    surface = cairo_xlib_surface_create
	(GDK_DRAWABLE_XDISPLAY(GDK_DRAWABLE(drawable->obj)),
	 GDK_DRAWABLE_XID(GDK_DRAWABLE(drawable->obj)),
	 GDK_VISUAL_XVISUAL(gdk_drawable_get_visual(GDK_DRAWABLE(drawable->obj))),
	 format,
	 GDK_COLORMAP_XCOLORMAP(gdk_drawable_get_colormap(GDK_DRAWABLE(drawable->obj))));
    if (!surface)
	return PyErr_NoMemory();

    return pycairo_surface_wrap(surface);
}

static PyObject *
set_target_drawable(PyObject *self, PyObject *args)
{
    PyCairoContext *context;
    PyGObject *drawable;
    GdkDrawable *real_drawable;
    gint x_offset, y_offset;

    if (!PyArg_ParseTuple(args, "O!O!:set_target_drawable",
			  &PyCairoContext_Type, &context,
			  &PyGdkDrawable_Type, &drawable))
	return NULL;

    if (GDK_IS_WINDOW(drawable->obj)) {
	gdk_window_get_internal_paint_info(GDK_WINDOW(drawable->obj),
					   &real_drawable,
					   &x_offset, &y_offset);
    } else {
	real_drawable = GDK_DRAWABLE(drawable->obj);
	x_offset = 0;
	y_offset = 0;
    }

    cairo_set_target_drawable(context->ctx,
			      GDK_DRAWABLE_XDISPLAY(real_drawable),
			      GDK_DRAWABLE_XID(real_drawable));
    if (pycairo_check_status(cairo_status(context->ctx)))
	return NULL;
    cairo_translate(context->ctx, -x_offset, -y_offset);
    if (pycairo_check_status(cairo_status(context->ctx)))
	return NULL;

    Py_RETURN_NONE;
}

static PyObject *
surface_create_for_pixbuf(PyObject *self, PyObject *args)
{
    PyGObject *py_pixbuf;
    GdkPixbuf *pixbuf;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!:surface_create_for_pixbuf",
			  &PyGdkPixbuf_Type, &py_pixbuf))
	return NULL;

    pixbuf = GDK_PIXBUF(py_pixbuf->obj);

    /* this is the only format that matches cairo's image format.
     * GdkPixbuf uses RGBA, while Cairo uses ARGB, so we can't handle
     * pixbufs with alpha. */
    if (gdk_pixbuf_get_colorspace(pixbuf) != GDK_COLORSPACE_RGB ||
	gdk_pixbuf_get_bits_per_sample(pixbuf) != 8 ||
	gdk_pixbuf_get_n_channels(pixbuf) != 4) {
	PyErr_SetString(PyExc_ValueError,
			"can only create a surface from a 24-bit RGB pixbuf "
			"(ie. no alpha)");
	return NULL;
    }

    surface = cairo_surface_create_for_image(gdk_pixbuf_get_pixels(pixbuf),
					     CAIRO_FORMAT_RGB24,
					     gdk_pixbuf_get_width(pixbuf),
					     gdk_pixbuf_get_height(pixbuf),
					     gdk_pixbuf_get_rowstride(pixbuf));
    if (!surface)
	return PyErr_NoMemory();
    /* should get surface to hold reference to pixbuf ... */

    return pycairo_surface_wrap(surface);
}

static PyMethodDef cairogtk_functions[] = {
    { "surface_create_for_drawable", (PyCFunction)surface_create_for_drawable,
      METH_VARARGS },
    { "set_target_drawable", (PyCFunction)set_target_drawable, METH_VARARGS },
    { "surface_create_for_pixbuf", (PyCFunction)surface_create_for_pixbuf,
      METH_VARARGS },
    { NULL, NULL, 0 }
};

DL_EXPORT(void)
initgtk(void)
{
    PyObject *mod;

    mod = Py_InitModule("cairo.gtk", cairogtk_functions);

    init_pycairo();
    init_pygtk();

    mod = PyImport_ImportModule("gtk.gdk");
    _PyGdkDrawable_Type = (PyTypeObject *)PyObject_GetAttrString(mod, "Drawable");
    _PyGdkPixbuf_Type = (PyTypeObject *)PyObject_GetAttrString(mod, "Pixbuf");
}
