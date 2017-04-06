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

import gtk
import cairo
import cairo.gtk

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
    width = widget.allocation.width
    height = widget.allocation.height

    if width < height:
        size = width
    else:
        size = height

    cr = cairo.Context()
    cairo.gtk.set_target_drawable(cr, widget.window)

    cr.translate ((width - size) / 2, (height - size) / 2)

    cr.scale(size / 150.0, size / 160.0)

    def man_hung():
        return len(letters_guessed_wrong) == body_parts

    cr.scale_font(10)
    cr.set_rgb_color(0,0,0)
    for idxLetter in range(len(word_chosen)):
        #print "Examining letter: " + word_chosen[idxLetter]
        if word_chosen[idxLetter].lower() in letters_guessed_right or man_hung():
            if idxLetter == 0:
                cr.move_to(0, 150)
                cr.show_text(word_chosen[idxLetter].upper())
            else:
                cr.move_to(idxLetter * 15, 150)
                cr.show_text(word_chosen[idxLetter].lower())
        else:
            cr.move_to(idxLetter * 15, 150)
            cr.show_text('_')
            
    # Draw Letters Guessed

    cr.move_to(0, 160)
    cr.set_rgb_color(0, 0, 0)
    cr.show_text(letters_guessed)

    # Draw noose

    cr.move_to(100, 12.5)
    cr.line_to(100, 5)
    cr.line_to(130, 5)
    cr.line_to(130, 100)
    cr.set_rgb_color(0, 0, 0)
    cr.stroke()

    # Draw base fill color

    cr.move_to(130, 100)
    cr.line_to(130, 140)
    cr.line_to(40, 140)
    cr.line_to(40, 130)
    cr.line_to(50, 130)
    cr.line_to(50, 122)
    cr.line_to(60, 122)
    cr.line_to(60, 114)
    cr.line_to(70, 114)
    cr.line_to(70, 106)
    cr.line_to(130, 106)
    cr.set_rgb_color(.4, .2, .1)
    cr.fill()

    # Draw base outline color

    cr.move_to(130, 100)
    cr.line_to(130, 140)
    cr.line_to(40, 140)

    # Draw 1st(lowest) stair

    cr.line_to(40, 130)
    cr.line_to(50, 130)
    cr.line_to(130, 130)
    cr.set_rgb_color(0, 0, 0)
    cr.stroke()
    
    # Draw 2nd stair

    cr.move_to(50, 130)
    cr.line_to(50, 121)
    cr.line_to(60, 121)
    cr.line_to(130, 121)
    cr.stroke()
    
    # Draw 3rd stair

    cr.move_to(60, 121)
    cr.line_to(60, 113)
    cr.line_to(70, 113)
    cr.line_to(130, 113)
    cr.stroke()
    
    # Draw 4th(top) stair
        
    cr.move_to(70, 113)
    cr.line_to(70, 105)
    cr.line_to(130, 105)
    cr.stroke()

    # Draw Head

    if len(letters_guessed_wrong) > 0:
        cr.move_to(107.5, 20)
        cr.arc(100, 20, 7.5, 0, 2*pi)
        cr.set_line_width(1)
        cr.stroke()

        # Draw Eye 1

        cr.move_to(104, 17)
        cr.arc(103, 17, 1, 0, 2*pi)
        cr.move_to(103.1, 17)
        cr.arc(103, 17, .1, 0, 2*pi)

        # Draw Eye 2

        cr.move_to(98, 17)
        cr.arc(97, 17, 1, 0, 2*pi)
        cr.move_to(97.1, 17)
        cr.arc(97, 17, .1, 0, 2*pi)

        # Draw Nose

        cr.move_to(100.5, 19)
        cr.line_to(99.5, 21)
        cr.line_to(100.5, 21)
    
        # Draw Mouth

        if len(letters_guessed_wrong) < 6:
            cr.move_to(97, 23)
            cr.curve_to(97, 23, 100, 27.5, 103, 23)
            cr.set_line_width(.5)
            cr.stroke()
        else:
            cr.move_to(100.5, 24)
            cr.arc(100, 24, .5, 0, 2*pi)
            cr.set_line_width(.5)
            cr.stroke()        

    cr.set_rgb_color(0, 0, 0)
    cr.set_line_width(1)

    # Draw Body

    if len(letters_guessed_wrong) > 1:
        cr.move_to(100, 27.5)
        cr.line_to(100, 70)
        cr.stroke()

    # Draw Arm 1

    if len(letters_guessed_wrong) > 2:
        cr.move_to(100, 35)
        cr.line_to(110, 50)
        cr.stroke()

    # Draw Arm 2

    if len(letters_guessed_wrong) > 3:
        cr.move_to(100, 35)
        cr.line_to(90, 50)
        cr.stroke()

    # Draw Leg 1

    if len(letters_guessed_wrong) > 4:
        cr.move_to(100, 70)
        cr.line_to(112, 95)
        cr.stroke()
        
    # Draw Leg 2

    if len(letters_guessed_wrong) > 5:
        cr.move_to(100, 70)
        cr.line_to(88, 95)
        cr.stroke()

win = gtk.Window()
win.set_title('Kevin\'s Cairo Demo')

drawingarea = gtk.DrawingArea()
drawingarea.set_size_request(300,320)
win.add(drawingarea)

win.show_all()

win.connect('destroy', gtk.main_quit)
win.connect('key_press_event', guess_letter)
drawingarea.connect('expose_event', expose_event)

gtk.main()
