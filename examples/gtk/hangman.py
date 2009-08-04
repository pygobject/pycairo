#!/usr/bin/env python
# -*- coding: iso8859-1 -*-
#
# Copyright © 2004 Kevin Worth
#
# Permission to use, copy, modify, distribute, and sell this software
# and its documentation for any purpose is hereby granted without fee,
# provided that the above copyright notice appear in all copies and
# that both that copyright notice and this permission notice appear in
# supporting documentation, and that the name of the Kevin Worth not
# be used in advertising or publicity pertaining to distribution of
# the software without specific, written prior permission. Kevin Worth
# California makes no representations about the suitability of this
# software for any purpose.  It is provided "as is" without express or
# implied warranty.
#
# KEVIN WORTH DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
# NO EVENT SHALL KEVIN WORTH BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
# OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Author: Kevin Worth <kevin@theworths.org>

from math import pi
from random import randint

import cairo
import gtk

letters_guessed = ""
letters_guessed_right = ""
letters_guessed_wrong = ""

body_parts = 6
words = ['cairo', 'graphics', 'pencil', 'keyboard', 'science', 'ricochet', 'flood', 'president', 'sanity']

idxRandom = randint(0, len(words) -1)
word_chosen = words[idxRandom]

#print "The secret word is " + word_chosen

def guess_letter(widget, event):
    global letters_guessed, letters_guessed_right, letters_guessed_wrong
    if event.string in letters_guessed:
        print "What?!?! You already guessed '" + event.string + "'!"
    else:
        letters_guessed += event.string
        if word_chosen.lower().find(event.string) != -1:
            letters_guessed_right += event.string
        else:
            letters_guessed_wrong += event.string
        widget.queue_draw()

def expose_event(widget, event):
    _, _, width, height = widget.allocation

    if width < height:
        size = width
    else:
        size = height

    pixmap = gtk.gdk.Pixmap (widget.window, width, height)
    ctx = pixmap.cairo_create()

    # set the background
    ctx.set_source_rgb(0.7,0.7,0.7)
    ctx.set_operator (cairo.OPERATOR_SOURCE)
    ctx.paint()

    ctx.translate ((width - size) / 2, (height - size) / 2)
    ctx.scale(size / 150.0, size / 160.0)

    def man_hung():
        return len(letters_guessed_wrong) == body_parts

    ctx.set_font_size(10)
    ctx.set_source_rgb(0,0,0)
    for idxLetter in range(len(word_chosen)):
        #print "Examining letter: " + word_chosen[idxLetter]
        if word_chosen[idxLetter].lower() in letters_guessed_right or man_hung():
            if idxLetter == 0:
                ctx.move_to(0, 150)
                ctx.show_text(word_chosen[idxLetter].upper())
            else:
                ctx.move_to(idxLetter * 15, 150)
                ctx.show_text(word_chosen[idxLetter].lower())
        else:
            ctx.move_to(idxLetter * 15, 150)
            ctx.show_text('_')

    # Draw Letters Guessed

    ctx.move_to(0, 160)
    ctx.set_source_rgb(0, 0, 0)
    ctx.show_text(letters_guessed)

    # Draw noose

    ctx.move_to(100, 12.5)
    ctx.line_to(100, 5)
    ctx.line_to(130, 5)
    ctx.line_to(130, 100)
    ctx.set_source_rgb(0, 0, 0)
    ctx.stroke()

    # Draw base fill color

    ctx.move_to(130, 100)
    ctx.line_to(130, 140)
    ctx.line_to(40, 140)
    ctx.line_to(40, 130)
    ctx.line_to(50, 130)
    ctx.line_to(50, 122)
    ctx.line_to(60, 122)
    ctx.line_to(60, 114)
    ctx.line_to(70, 114)
    ctx.line_to(70, 106)
    ctx.line_to(130, 106)
    ctx.set_source_rgb(.4, .2, .1)
    ctx.fill()

    # Draw base outline color

    ctx.move_to(130, 100)
    ctx.line_to(130, 140)
    ctx.line_to(40, 140)

    # Draw 1st(lowest) stair

    ctx.line_to(40, 130)
    ctx.line_to(50, 130)
    ctx.line_to(130, 130)
    ctx.set_source_rgb(0, 0, 0)
    ctx.stroke()

    # Draw 2nd stair

    ctx.move_to(50, 130)
    ctx.line_to(50, 121)
    ctx.line_to(60, 121)
    ctx.line_to(130, 121)
    ctx.stroke()

    # Draw 3rd stair

    ctx.move_to(60, 121)
    ctx.line_to(60, 113)
    ctx.line_to(70, 113)
    ctx.line_to(130, 113)
    ctx.stroke()

    # Draw 4th(top) stair

    ctx.move_to(70, 113)
    ctx.line_to(70, 105)
    ctx.line_to(130, 105)
    ctx.stroke()

    # Draw Head

    if len(letters_guessed_wrong) > 0:
        ctx.move_to(107.5, 20)
        ctx.arc(100, 20, 7.5, 0, 2*pi)
        ctx.set_line_width(1)
        ctx.stroke()

        # Draw Eye 1

        ctx.move_to(104, 17)
        ctx.arc(103, 17, 1, 0, 2*pi)
        ctx.move_to(103.1, 17)
        ctx.arc(103, 17, .1, 0, 2*pi)

        # Draw Eye 2

        ctx.move_to(98, 17)
        ctx.arc(97, 17, 1, 0, 2*pi)
        ctx.move_to(97.1, 17)
        ctx.arc(97, 17, .1, 0, 2*pi)

        # Draw Nose

        ctx.move_to(100.5, 19)
        ctx.line_to(99.5, 21)
        ctx.line_to(100.5, 21)

        # Draw Mouth

        if len(letters_guessed_wrong) < 6:
            ctx.move_to(97, 23)
            ctx.curve_to(97, 23, 100, 27.5, 103, 23)
            ctx.set_line_width(.5)
            ctx.stroke()
        else:
            ctx.move_to(100.5, 24)
            ctx.arc(100, 24, .5, 0, 2*pi)
            ctx.set_line_width(.5)
            ctx.stroke()

    ctx.set_source_rgb(0, 0, 0)
    ctx.set_line_width(1)

    # Draw Body

    if len(letters_guessed_wrong) > 1:
        ctx.move_to(100, 27.5)
        ctx.line_to(100, 70)
        ctx.stroke()

    # Draw Arm 1

    if len(letters_guessed_wrong) > 2:
        ctx.move_to(100, 35)
        ctx.line_to(110, 50)
        ctx.stroke()

    # Draw Arm 2

    if len(letters_guessed_wrong) > 3:
        ctx.move_to(100, 35)
        ctx.line_to(90, 50)
        ctx.stroke()

    # Draw Leg 1

    if len(letters_guessed_wrong) > 4:
        ctx.move_to(100, 70)
        ctx.line_to(112, 95)
        ctx.stroke()

    # Draw Leg 2

    if len(letters_guessed_wrong) > 5:
        ctx.move_to(100, 70)
        ctx.line_to(88, 95)
        ctx.stroke()

    # draw pixmap to gdk.window
    gc = gtk.gdk.GC(widget.window)
    widget.window.draw_drawable(gc, pixmap, 0,0, 0,0, -1,-1)


win = gtk.Window()
win.connect('destroy', gtk.main_quit)
win.connect('key_press_event', guess_letter)
win.set_title('Kevin\'s cairo demo')

drawingarea = gtk.DrawingArea()
win.add(drawingarea)
drawingarea.connect('expose_event', expose_event)
drawingarea.set_size_request(300,320)

win.show_all()
gtk.main()
