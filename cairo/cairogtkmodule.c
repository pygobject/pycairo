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
#include <pygtk/pygtk.h>
#include <gdk/gdkx.h>

#include "pycairo.h"

#ifdef CAIRO_HAS_XLIB_SURFACE
#  include <cairo-xlib.h>
#else
#  error "Cairo was not compiled with support for the xlib backend"
#endif


static Pycairo_CAPI_t *Pycairo_CAPI;

static PyTypeObject *_PyGdkDrawable_Type;
#define PyGdkDrawable_Type (*_PyGdkDrawable_Type)
static PyTypeObject *_PyGdkPixbuf_Type;
#define PyGdkPixbuf_Type (*_PyGdkPixbuf_Type)

static PyObject *
surface_create_for_pixbuf(PyObject *self, PyObject *args)
{
    PyGObject *py_pixbuf;
    GdkPixbuf *gdk_pixbuf;
    cairo_surface_t *surface;

    if (!PyArg_ParseTuple(args, "O!:surface_create_for_pixbuf",
			  &PyGdkPixbuf_Type, &py_pixbuf))
	return NULL;

    gdk_pixbuf = GDK_PIXBUF(py_pixbuf->obj);

    /* this is the only format that matches cairo's image format.
     * GdkPixbuf uses RGBA, while Cairo uses ARGB, so we can't handle
     * pixbufs with alpha. */
    if (gdk_pixbuf_get_colorspace(gdk_pixbuf) != GDK_COLORSPACE_RGB ||
	gdk_pixbuf_get_bits_per_sample(gdk_pixbuf) != 8 ||
	gdk_pixbuf_get_n_channels(gdk_pixbuf) != 4) {
	PyErr_SetString(PyExc_ValueError,
			"can only create a surface from a 24-bit RGB pixbuf "
			"(ie. no alpha)");
	return NULL;
    }
    surface = cairo_image_surface_create_for_data
	(gdk_pixbuf_get_pixels(gdk_pixbuf),
	 CAIRO_FORMAT_RGB24,
	 gdk_pixbuf_get_width(gdk_pixbuf),
	 gdk_pixbuf_get_height(gdk_pixbuf),
	 gdk_pixbuf_get_rowstride(gdk_pixbuf));
    if (!surface)
	return PyErr_NoMemory();
    return PycairoSurface_FromSurface (surface, &PycairoImageSurface_Type,
				       (PyObject *)py_pixbuf);
}

#ifndef HAVE_GTK28
/* copied from gtk+/gdk/gdkcairo.c and gtk+/gdk/x11/gdkdrawable-x11.c
 * gdk_cairo_create() should be available in gtk 2.8
 */
static cairo_t *
gdk_cairo_create (GdkDrawable *drawable)
{
    int width, height;
    cairo_t *cr = NULL;
    cairo_surface_t *surface = NULL;
    GdkVisual *visual = gdk_drawable_get_visual (drawable);

    gdk_drawable_get_size (drawable, &width, &height);
    if (visual) 
	surface = cairo_xlib_surface_create (GDK_DRAWABLE_XDISPLAY (drawable),
					     GDK_DRAWABLE_XID (drawable),
					     GDK_VISUAL_XVISUAL (visual),
					     width, height);
    else if (gdk_drawable_get_depth (drawable) == 1)
	surface = cairo_xlib_surface_create_for_bitmap 
	    (GDK_PIXMAP_XDISPLAY (drawable),
	     GDK_PIXMAP_XID (drawable),
	     width, height);
    else {
	g_warning ("Using Cairo rendering requires the drawable argument to\n"
		   "have a specified colormap. All windows have a colormap,\n"
		   "however, pixmaps only have colormap by default if they\n"
		   "were created with a non-NULL window argument. Otherwise\n"
		   "a colormap must be set on them with "
		   "gdk_drawable_set_colormap");
	return NULL;
    }
    if (surface) {
	cr = cairo_create (surface);
	cairo_surface_destroy (surface);
    }
    return cr;
}
#endif

/* gdk.cairo_create() should be available in pygtk 2.8 */
static PyObject *
pygdk_cairo_create(PyObject *self, PyObject *args)
{
    cairo_t *cr;
    PyGObject *py_drawable;

    if (!PyArg_ParseTuple(args, "O!:gdk_cairo_create",
			  &PyGdkDrawable_Type, &py_drawable))
	return NULL;

    cr = gdk_cairo_create (GDK_DRAWABLE(py_drawable->obj));
    return PycairoContext_FromContext (cr, NULL, (PyObject *)py_drawable);
}

static PyMethodDef cairogtk_functions[] = {
    { "gdk_cairo_create", (PyCFunction)pygdk_cairo_create,     METH_VARARGS },
    { "surface_create_for_pixbuf", 
      (PyCFunction)surface_create_for_pixbuf,                  METH_VARARGS },
    { NULL, NULL, 0 }
};


DL_EXPORT(void)
initgtk(void)
{
    PyObject *mod = Py_InitModule("cairo.gtk", cairogtk_functions);
    if (!mod)
	return;

    Pycairo_IMPORT;

    /* strange way to access the pygtk C API, why not PyGTK_IMPORT ? */
    init_pygtk();
    mod = PyImport_ImportModule("gtk.gdk");
    if (!mod)
	return;
    _PyGdkDrawable_Type = (PyTypeObject *)PyObject_GetAttrString(mod, 
								 "Drawable");
    _PyGdkPixbuf_Type   = (PyTypeObject *)PyObject_GetAttrString(mod, 
								 "Pixbuf");
}
