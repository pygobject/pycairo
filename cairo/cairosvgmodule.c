/* -*- mode: C; c-basic-offset: 4 -*- 
 *
 * PyCairo - Python bindings for Cairo
 *
 * Copyright © 2003-2004 Steve Chaplin
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
 *                 Steve Chaplin
 */

#include <Python.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "pycairosvg-private.h"
#include "pycairo.h"


/* cairo.svg module
 * - no module methods
 * - one module type: cairo.svg.Context()
*/


static PyMethodDef svg_methods[] = {
    {NULL},
};


PyDoc_STRVAR(svg_doc,
"libsvg-cairo bindings."
);

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initsvg (void)
{
    PyObject* mod;

    if (PyType_Ready(&PyCairoSVGContext_Type) < 0)
        return;

    mod = Py_InitModule3 ("cairo.svg", svg_methods, svg_doc);
    if (mod == NULL)
	return;

    init_pycairo();

    Py_INCREF(&PyCairoSVGContext_Type);
    PyModule_AddObject(mod, "Context", (PyObject *)&PyCairoSVGContext_Type);
}
