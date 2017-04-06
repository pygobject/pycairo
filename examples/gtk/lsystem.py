#!/usr/bin/env python

import cairo
import gtk

# Copyright 2003 Jesse Andrews (jdandr2@uky.edu) under GPL


class lindenmayer:
    def __init__( self ):
        self.str = ''
        self.prod = {'[':'[','f':'f',']':']','+':'+','-':'-'}
        self.SIZE = 10
        self.THETA = 90

    def addProd( self, let, pro ):
        self.prod[let]=pro

    def iterate( self, qty=1 ):
        for i in xrange(qty):
            self.str = ''.join([ self.prod[l] for l in self.str])
        print 'Done iterating'

    def expose( self, drawingarea, event ):
        drawable = drawingarea.window
        x, y, width, height = drawingarea.allocation

        ctx = drawable.cairo_create()
        ctx.set_source_rgb(0, 0, 0)

        ctx.set_line_width(self.SIZE / 4)
        ctx.set_tolerance(0.1)
        ctx.set_line_join(cairo.LINE_JOIN_BEVEL)

        ctx.new_path()
        ctx.move_to(100,100)

        for c in self.str:
            if c == 'f': line(ctx, self.SIZE )
            if c == '+': rotate( ctx, +self.THETA )
            if c == '-': rotate( ctx, -self.THETA )
            if c == '[': ctx.save()
            if c == ']': ctx.restore()

        ctx.stroke()

def line(ctx, len):
    ctx.rel_line_to( 0, len )

def rotate(ctx, deg):
    ctx.rotate( 2*3.141592653589793*deg/360.0  )

def lin_setup():
    cls = lindenmayer()
    ################# SETUP LSYSTEM HERE ################

    ### Generic stuff ###

    cls.str = 'f'   # the starting string

    cls.SIZE = 5    # length of a line

    ##############################################
    ##############################################
    #### Uncomment the one you want to use... ####
    #### only one at a time right now!        ####
    ##############################################
    ##############################################

    ###### Kock Square Curve #######
    cls.addProd('f','f-f+f+f-f')
    cls.THETA = 90

    ###### Kock Snowflake ######

#    cls.addProd('f','f-f++f-f')
#    cls.THETA = 60

    ######## Peano Curve ########
#    cls.addProd('x', 'xfyfx+f+yfxfy-f-xfyfx')
#    cls.addProd('y', 'yfxfy-f-xfyfx+f+yfxfy')
#    cls.addProd('f', 'f')
#    cls.THETA = 90
#    cls.str = 'y'

    ###### the plant ######
    ## doesn't seem to work ... .save & .restore messed up ##

#    cls.addProd( 'f','f[+f]f[-f]f' )
#    cls.THETA = 25

    ####### the tree #########
    ## doesn't seem to work ... .save & .restore messed up ##

#    cls.addProd( 'f', 'ff+[+f-f-f]-[-f+f+f]' )
#    cls.THETA = 22


    ### times to iterate string rewriting ###
    #this grows QUICKLY, so start only inc by 1 each run!
    cls.iterate(4)

    ################ DONE SETUP ###############
    return cls

def main():
    win = gtk.Window()
    win.connect('destroy', lambda x: gtk.main_quit())
    win.set_title('cairo Lindenmayer System')
    win.set_default_size(600, 600)

    cls = lin_setup()

    drawingarea = gtk.DrawingArea()
    win.add(drawingarea)
    drawingarea.connect('expose_event', cls.expose)

    win.show_all()
    gtk.main()

if __name__ == '__main__':
    main()

