/* Copyright 2018 Bernhard R. Fischer, 4096R/8E24F29D <bf@abenteuerland.at>
 *
 * This file is part of Cairo_JPG.
 *
 * Cairo_JPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cairo_JPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cairo_JPG.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CAIRO_JPEG_H
#define CAIRO_JPEG_H

/*! \file cairo_jpg.h
 * This file contains all prototypes for the Cairo-JPEG functions implemented
 * in cairo_jpg.c. See there for the function documentation.
 *
 * @author Bernhard R. Fischer, 4096R/8E24F29D <bf@abenteuerland.at>
 * @version 2018/12/11
 * @license LGPL3
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cairo.h>

#ifdef USE_CAIRO_READ_FUNC_LEN_T
/*! This is the type for the stream read function. Which must be implemented by
 * the user if cairo_image_surface_create_from_jpeg_stream() is used. Please
 * note that this prototype is slightly different from cairo_read_func_t which
 * is used by cairo_image_surface_create_from_png_stream().
 * This new prototype is defined because the original prototype
 * cairo_read_func_t does not allow to detect truncated reads. This issue was
 * discussed on the cairographics mailinglist, see
 * https://lists.cairographics.org/archives/cairo/2016-March/027298.html
 * @param closure This parameter is directly passed through by
 * cairo_image_surface_create_from_jpeg_stream().
 * @param data Pointer to data buffer which will receive the data.
 * @param length Size of the data buffer in bytes.
 * @return This function must return the actually length that was read into the
 * buffer. This may actually be less than length which indicates an EOF. In
 * case of any fatal unrecoverable error on the input stream -1 shall be
 * returned.
 */
typedef ssize_t (*cairo_read_func_len_t) (void *closure, unsigned char *data, unsigned int length);
#endif


cairo_status_t cairo_image_surface_write_to_jpeg_mem(cairo_surface_t *sfc, unsigned char **data, size_t *len, int quality);
cairo_status_t cairo_image_surface_write_to_jpeg_stream(cairo_surface_t *sfc, cairo_write_func_t write_func, void *closure, int quality);
cairo_status_t cairo_image_surface_write_to_jpeg(cairo_surface_t *sfc, const char *filename, int quality);
cairo_surface_t *cairo_image_surface_create_from_jpeg_mem(void *data, size_t len);
#ifdef USE_CAIRO_READ_FUNC_LEN_T
cairo_surface_t *cairo_image_surface_create_from_jpeg_stream(cairo_read_func_len_t read_func, void *closure);
#else
cairo_surface_t *cairo_image_surface_create_from_jpeg_stream(cairo_read_func_t read_func, void *closure);
#endif
cairo_surface_t *cairo_image_surface_create_from_jpeg(const char *filename);

#endif

