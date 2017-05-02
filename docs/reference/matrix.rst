.. _matrix:

******
Matrix
******

.. currentmodule:: cairo


class Matrix()
==============

*Matrix* is used throughout cairo to convert between different coordinate
spaces.  A *Matrix* holds an affine transformation, such as a scale, rotation,
shear, or a combination of these.  The transformation of a point (x,y) is
given by::

  x_new = xx * x + xy * y + x0
  y_new = yx * x + yy * y + y0

The current transformation matrix of a :class:`Context`, represented as a
*Matrix*, defines the transformation from user-space coordinates to device-space
coordinates.

Some standard Python operators can be used with matrices:

To read the values from a *Matrix*::

  xx, yx, xy, yy, x0, y0 = matrix

To multiply two matrices::

  matrix3 = matrix1.multiply(matrix2)
  # or equivalently
  matrix3 = matrix1 * matrix2

To compare two matrices::

  matrix1 == matrix2
  matrix1 != matrix2

For more information on matrix transformation see https://www.cairographics.org/cookbook/matrix_transform/


.. class:: Matrix(xx = 1.0, yx = 0.0, xy = 0.0, yy = 1.0, x0 = 0.0, y0 = 0.0)

   :param xx: xx component of the affine transformation
   :type xx: float
   :param yx: yx component of the affine transformation
   :type yx: float
   :param xy: xy component of the affine transformation
   :type xy: float
   :param yy: yy component of the affine transformation
   :type yy: float
   :param x0: X translation component of the affine transformation
   :type x0: float
   :param y0: Y translation component of the affine transformation
   :type y0: float

   Create a new *Matrix* with the affine transformation given by *xx, yx, xy,
   yy, x0, y0*. The transformation is given by::

     x_new = xx * x + xy * y + x0
     y_new = yx * x + yy * y + y0

   To create a new identity matrix::

     matrix = cairo.Matrix()

   To create a matrix with a transformation which translates by tx and ty in the X and Y dimensions, respectively::

     matrix = cairo.Matrix(x0=tx, y0=ty)

   To create a matrix with a transformation that scales by sx and sy in the X and Y dimensions, respectively::

     matrix = cairo.Matrix(xx=sy, yy=sy)


   .. classmethod:: init_rotate(radians)

      :param radians: angle of rotation, in radians. The direction of rotation
        is defined such that positive angles rotate in the direction from the
        positive X axis toward the positive Y axis. With the default axis
        orientation of cairo, positive angles rotate in a clockwise direction.
      :type radians: float
      :returns: a new *Matrix* set to a transformation that rotates by *radians*.


   .. method:: invert()

      :returns: If *Matrix* has an inverse, modifies *Matrix* to be the
        inverse matrix and returns *None*
      :raises: :exc:`cairo.Error` if the *Matrix* as no inverse

      Changes *Matrix* to be the inverse of it's original value. Not all
      transformation matrices have inverses; if the matrix collapses points
      together (it is *degenerate*), then it has no inverse and this function
      will fail.


   .. method:: multiply(matrix2)

      :param matrix2: a second matrix
      :type matrix2: cairo.Matrix
      :returns: a new *Matrix*

      Multiplies the affine transformations in *Matrix* and *matrix2*
      together. The effect of the resulting transformation is to first apply
      the transformation in *Matrix* to the coordinates and then apply the
      transformation in *matrix2* to the coordinates.

      It is allowable for result to be identical to either *Matrix* or *matrix2*.


   .. method:: rotate(radians)

      :param radians: angle of rotation, in radians. The direction of rotation
         is defined such that positive angles rotate in the direction from the
         positive X axis toward the positive Y axis. With the default axis
         orientation of cairo, positive angles rotate in a clockwise direction.
      :type radians: float

      Initialize *Matrix* to a transformation that rotates by *radians*.

   .. method:: scale(sx, sy)

      :param sx: scale factor in the X direction
      :type sx: float
      :param sy: scale factor in the Y direction
      :type sy: float

      Applies scaling by *sx, sy* to the transformation in *Matrix*. The
      effect of the new transformation is to first scale the coordinates by
      *sx* and *sy*, then apply the original transformation to the
      coordinates.

   .. method:: transform_distance(dx, dy)

      :param dx: X component of a distance vector.
      :type dx: float
      :param dy: Y component of a distance vector.
      :type dy: float
      :returns: the transformed distance vector (dx,dy), both float
      :rtype: tuple

      Transforms the distance vector *(dx,dy)* by *Matrix*. This is similar to
      :meth:`.transform_point` except that the translation components of
      the transformation are ignored. The calculation of the returned vector
      is as follows::

        dx2 = dx1 * a + dy1 * c
        dy2 = dx1 * b + dy1 * d

      Affine transformations are position invariant, so the same vector always
      transforms to the same vector. If *(x1,y1)* transforms to *(x2,y2)* then
      *(x1+dx1,y1+dy1)* will transform to *(x1+dx2,y1+dy2)* for all values
      of *x1* and *x2*.


   .. method:: transform_point(x, y)

      :param x: X position.
      :type x: float
      :param y: Y position.
      :type y: float
      :returns: the transformed point (x,y), both float
      :rtype: tuple

      Transforms the point *(x, y)* by *Matrix*.

   .. method:: translate(tx, ty)

      :param tx: amount to translate in the X direction
      :type tx: float
      :param ty: amount to translate in the Y direction
      :type ty: float

      Applies a transformation by *tx, ty* to the transformation in
      *Matrix*. The effect of the new transformation is to first translate the
      coordinates by *tx* and *ty*, then apply the original transformation to the
      coordinates.

   .. data:: xx

      :class:`float`: xx component of the affine transformation

      .. versionadded:: 1.12.0

   .. data:: yx

      :class:`float`: yx component of the affine transformation

      .. versionadded:: 1.12.0

   .. data:: xy

      :class:`float`: xy component of the affine transformation

      .. versionadded:: 1.12.0

   .. data:: yy

      :class:`float`: yy component of the affine transformation

      .. versionadded:: 1.12.0

   .. data:: x0

      :class:`float`: X translation component of the affine transformation

      .. versionadded:: 1.12.0

   .. data:: y0

      :class:`float`: Y translation component of the affine transformation

      .. versionadded:: 1.12.0
