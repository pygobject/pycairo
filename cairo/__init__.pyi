from typing import (Any, BinaryIO, ByteString, Callable, List, Optional,
                    Sequence, Text, Tuple, TypeVar, Union)

HAS_ATSUI_FONT: bool = ...
HAS_FT_FONT: bool = ...
HAS_GLITZ_SURFACE: bool = ...
HAS_IMAGE_SURFACE: bool = ...
HAS_MIME_SURFACE: bool = ...
HAS_PDF_SURFACE: bool = ...
HAS_PNG_FUNCTIONS: bool = ...
HAS_PS_SURFACE: bool = ...
HAS_QUARTZ_SURFACE: bool = ...
HAS_RECORDING_SURFACE: bool = ...
HAS_SCRIPT_SURFACE: bool = ...
HAS_SVG_SURFACE: bool = ...
HAS_TEE_SURFACE: bool = ...
HAS_USER_FONT: bool = ...
HAS_WIN32_FONT: bool = ...
HAS_WIN32_SURFACE: bool = ...
HAS_XCB_SURFACE: bool = ...
HAS_XLIB_SURFACE: bool = ...

PDF_OUTLINE_ROOT: int = ...

version: str = ...
version_info: Tuple[int, int, int] = ...

CAIRO_VERSION: int = ...
CAIRO_VERSION_STRING: str = ...
CAIRO_VERSION_MAJOR: int = ...
CAIRO_VERSION_MINOR: int = ...
CAIRO_VERSION_MICRO: int = ...

def cairo_version() -> int:
    """
    Returns the version of the underlying C cairo library, encoded in a single integer.
    """
def cairo_version_string() -> str:
    """
    Returns the version of the underlying C cairo library as a human-readable string of the form “X.Y.Z”.
    """

class Path:
    """
    *Path* cannot be instantiated directly, it is created by calling
    :meth:`Context.copy_path` and :meth:`Context.copy_path_flat`.

    str(path) lists the path elements.

    See :class:`path attributes <cairo.PathDataType>`

    Path is an iterator.

    See examples/warpedtext.py for example usage.
    """

class Rectangle(Tuple[float, float, float, float]):
    """
    .. versionadded:: 1.15
        In prior versions a (float, float, float, float) tuple was
        used instead of :class:`Rectangle`.

    A data structure for holding a rectangle.
    """

    x: float = ...
    y: float = ...
    width: float = ...
    height: float = ...

    def __init__(self, x: float, y: float, width: float, height: float) -> None:
        """
        :param x:
            X coordinate of the left side of the rectangle
        :param y:
            Y coordinate of the the top side of the rectangle
        :param width:
            width of the rectangle
        :param height:
            height of the rectangle
        """

class _IntEnum(int):
    def __init__(self, value: int) -> None: ...

class Antialias(_IntEnum):
    """
    Specifies the type of antialiasing to do when rendering text or shapes.

    New in version 1.13.
    """
    BEST: "Antialias" = ...
    DEFAULT: "Antialias" = ...
    FAST: "Antialias" = ...
    GOOD: "Antialias" = ...
    GRAY: "Antialias" = ...
    NONE: "Antialias" = ...
    SUBPIXEL: "Antialias" = ...

class Content(_IntEnum):
    """
    These constants are used to describe the content that a Surface will contain, whether color information, alpha information (translucence vs. opacity), or both.

    New in version 1.13.
    """
    ALPHA: "Content" = ...
    COLOR: "Content" = ...
    COLOR_ALPHA: "Content" = ...

class FillRule(_IntEnum):
    """
    These constants are used to select how paths are filled. For both fill rules, whether or not a point is included in the fill is determined by taking a ray from that point to infinity and looking at intersections with the path. The ray can be in any direction, as long as it doesn’t pass through the end point of a segment or have a tricky intersection such as intersecting tangent to the path. (Note that filling is not actually implemented in this way. This is just a description of the rule that is applied.)

    The default fill rule is `WINDING`.

    New in version 1.13
    """
    EVEN_ODD: "FillRule" = ...
    WINDING: "FillRule" = ...

class Format(_IntEnum):
    """
    These constants are used to identify the memory format of `ImageSurface` data.

    New entries may be added in future versions.

    New in version 1.13.
    """
    def stride_for_width(self, width: int) -> int:
        """
        This method provides a stride value that will respect all alignment requirements of the accelerated image-rendering code within cairo. Typical usage will be of the form:

        >>> format = cairo.Format.RGB24
        >>> 
        >>>     stride = format.stride_for_width(width)
        >>> 
        >>>     surface = cairo.ImageSurface.create_for_data(
        >>>         data, format, width, height, stride)

        Also available under cairo.ImageSurface.format_stride_for_width().

        New in version 1.14.
        """
    A1: "Format" = ...
    A8: "Format" = ...
    ARGB32: "Format" = ...
    INVALID: "Format" = ...
    RGB16_565: "Format" = ...
    RGB24: "Format" = ...
    RGB30: "Format" = ...

class HintMetrics(_IntEnum):
    """
    These constants specify whether to hint font metrics; hinting font metrics means quantizing them so that they are integer values in device space. Doing this improves the consistency of letter and line spacing, however it also means that text will be laid out differently at different zoom factors.

    New in version 1.13.
    """
    DEFAULT: "HintMetrics" = ...
    OFF: "HintMetrics" = ...
    ON: "HintMetrics" = ...

class HintStyle(_IntEnum):
    """
    These constants specify the type of hinting to do on font outlines. Hinting is the process of fitting outlines to the pixel grid in order to improve the appearance of the result. Since hinting outlines involves distorting them, it also reduces the faithfulness to the original outline shapes. Not all of the outline hinting styles are supported by all font backends.

    New entries may be added in future versions.

    New in version 1.13.
    """
    DEFAULT: "HintStyle" = ...
    FULL: "HintStyle" = ...
    MEDIUM: "HintStyle" = ...
    NONE: "HintStyle" = ...
    SLIGHT: "HintStyle" = ...

class SubpixelOrder(_IntEnum):
    """
    The subpixel order specifies the order of color elements within each pixel on the display device when rendering with an antialiasing mode of `Antialias.SUBPIXEL`.

    New in version 1.13.
    """
    BGR: "SubpixelOrder" = ...
    DEFAULT: "SubpixelOrder" = ...
    RGB: "SubpixelOrder" = ...
    VBGR: "SubpixelOrder" = ...
    VRGB: "SubpixelOrder" = ...

class LineCap(_IntEnum):
    """
    These constants specify how to render the endpoints of the path when stroking.

    The default line cap style is `BUTT`

    New in version 1.13
    """
    BUTT: "LineCap" = ...
    ROUND: "LineCap" = ...
    SQUARE: "LineCap" = ...

class LineJoin(_IntEnum):
    """
    These constants specify how to render the junction of two lines when stroking.

    The default line join style is `MITER`

    New in version 1.13.
    """
    BEVEL: "LineJoin" = ...
    MITER: "LineJoin" = ...
    ROUND: "LineJoin" = ...

class Filter(_IntEnum):
    """
    These constants are used to indicate what filtering should be applied when reading pixel values from patterns. See `Pattern`.`set_filter()` for indicating the desired filter to be used with a particular pattern.

    New in version 1.13.
    """
    BEST: "Filter" = ...
    BILINEAR: "Filter" = ...
    FAST: "Filter" = ...
    GAUSSIAN: "Filter" = ...
    GOOD: "Filter" = ...
    NEAREST: "Filter" = ...

class Operator(_IntEnum):
    """
    These constants are used to set the compositing operator for all cairo drawing operations.

    The default operator is `OVER`.

    The operators marked as unbounded modify their destination even outside of the mask layer (that is, their effect is not bound by the mask layer). However, their effect can still be limited by way of clipping.

    To keep things simple, the operator descriptions here document the behavior for when both source and destination are either fully transparent or fully opaque. The actual implementation works for translucent layers too.

    For a more detailed explanation of the effects of each operator, including the mathematical definitions, see https://cairographics.org/operators.

    New in version 1.13.
    """
    ADD: "Operator" = ...
    ATOP: "Operator" = ...
    CLEAR: "Operator" = ...
    COLOR_BURN: "Operator" = ...
    COLOR_DODGE: "Operator" = ...
    DARKEN: "Operator" = ...
    DEST: "Operator" = ...
    DEST_ATOP: "Operator" = ...
    DEST_IN: "Operator" = ...
    DEST_OUT: "Operator" = ...
    DEST_OVER: "Operator" = ...
    DIFFERENCE: "Operator" = ...
    EXCLUSION: "Operator" = ...
    HARD_LIGHT: "Operator" = ...
    HSL_COLOR: "Operator" = ...
    HSL_HUE: "Operator" = ...
    HSL_LUMINOSITY: "Operator" = ...
    HSL_SATURATION: "Operator" = ...
    IN: "Operator" = ...
    LIGHTEN: "Operator" = ...
    MULTIPLY: "Operator" = ...
    OUT: "Operator" = ...
    OVER: "Operator" = ...
    OVERLAY: "Operator" = ...
    SATURATE: "Operator" = ...
    SCREEN: "Operator" = ...
    SOFT_LIGHT: "Operator" = ...
    SOURCE: "Operator" = ...
    XOR: "Operator" = ...

class Extend(_IntEnum):
    """
    These constants are used to describe how `Pattern` color/alpha will be determined for areas “outside” the pattern’s natural area, (for example, outside the surface bounds or outside the gradient geometry).

    The default extend mode is `NONE` for `SurfacePattern` and `PAD` for `Gradient` patterns.

    New in version 1.13.
    """
    NONE: "Extend" = ...
    PAD: "Extend" = ...
    REFLECT: "Extend" = ...
    REPEAT: "Extend" = ...

class FontSlant(_IntEnum):
    """
    These constants specify variants of a FontFace based on their slant.

    New in version 1.13.
    """
    ITALIC: "FontSlant" = ...
    NORMAL: "FontSlant" = ...
    OBLIQUE: "FontSlant" = ...

class FontWeight(_IntEnum):
    """
    These constants specify variants of a `FontFace` based on their weight.

    New in version 1.13.
    """
    BOLD: "FontWeight" = ...
    NORMAL: "FontWeight" = ...

class Status(_IntEnum):
    CLIP_NOT_REPRESENTABLE: "Status" = ...
    DEVICE_ERROR: "Status" = ...
    DEVICE_FINISHED: "Status" = ...
    DEVICE_TYPE_MISMATCH: "Status" = ...
    FILE_NOT_FOUND: "Status" = ...
    FONT_TYPE_MISMATCH: "Status" = ...
    INVALID_CLUSTERS: "Status" = ...
    INVALID_CONTENT: "Status" = ...
    INVALID_DASH: "Status" = ...
    INVALID_DSC_COMMENT: "Status" = ...
    INVALID_FORMAT: "Status" = ...
    INVALID_INDEX: "Status" = ...
    INVALID_MATRIX: "Status" = ...
    INVALID_MESH_CONSTRUCTION: "Status" = ...
    INVALID_PATH_DATA: "Status" = ...
    INVALID_POP_GROUP: "Status" = ...
    INVALID_RESTORE: "Status" = ...
    INVALID_SIZE: "Status" = ...
    INVALID_SLANT: "Status" = ...
    INVALID_STATUS: "Status" = ...
    INVALID_STRIDE: "Status" = ...
    INVALID_STRING: "Status" = ...
    INVALID_VISUAL: "Status" = ...
    INVALID_WEIGHT: "Status" = ...
    JBIG2_GLOBAL_MISSING: "Status" = ...
    LAST_STATUS: "Status" = ...
    NEGATIVE_COUNT: "Status" = ...
    NO_CURRENT_POINT: "Status" = ...
    NO_MEMORY: "Status" = ...
    NULL_POINTER: "Status" = ...
    PATTERN_TYPE_MISMATCH: "Status" = ...
    READ_ERROR: "Status" = ...
    SUCCESS: "Status" = ...
    SURFACE_FINISHED: "Status" = ...
    SURFACE_TYPE_MISMATCH: "Status" = ...
    TEMP_FILE_ERROR: "Status" = ...
    USER_FONT_ERROR: "Status" = ...
    USER_FONT_IMMUTABLE: "Status" = ...
    USER_FONT_NOT_IMPLEMENTED: "Status" = ...
    WRITE_ERROR: "Status" = ...
    TAG_ERROR: "Status" = ...
    FREETYPE_ERROR: "Status" = ...
    PNG_ERROR: "Status" = ...
    WIN32_GDI_ERROR: "Status" = ...

class PDFVersion(_IntEnum):
    """
    These constants are used to describe the version number of the PDF specification that a generated PDF file will conform to.

    New in version 1.13.
    """
    VERSION_1_4: "PDFVersion" = ...
    VERSION_1_5: "PDFVersion" = ...

class PSLevel(_IntEnum):
    """
    These constants are used to describe the language level of the PostScript Language Reference that a generated PostScript file will conform to. Note: the constants are only defined when cairo has been compiled with PS support enabled.

    New in version 1.13.
    """
    LEVEL_2: "PSLevel" = ...
    LEVEL_3: "PSLevel" = ...

class PathDataType(_IntEnum):
    """
    These constants are used to describe the type of one portion of a path when represented as a `Path`.

    New in version 1.13.
    """
    CLOSE_PATH: "PathDataType" = ...
    CURVE_TO: "PathDataType" = ...
    LINE_TO: "PathDataType" = ...
    MOVE_TO: "PathDataType" = ...

class RegionOverlap(_IntEnum):
    """
    New in version 1.13
    """
    IN: "RegionOverlap" = ...
    OUT: "RegionOverlap" = ...
    PART: "RegionOverlap" = ...

class SVGVersion(_IntEnum):
    """
    These constants are used to describe the version number of the SVG specification that a generated SVG file will conform to.

    New in version 1.13.
    """
    VERSION_1_1: "SVGVersion" = ...
    VERSION_1_2: "SVGVersion" = ...

class SVGUnit(_IntEnum):
    """
    SVGUnit is used to describe the units valid for coordinates and lengths in the SVG specification.

    See also:
    ---------
    - https://www.w3.org/TR/SVG/coords.htmlUnits
    - https://www.w3.org/TR/SVG/types.htmlDataTypeLength
    - https://www.w3.org/TR/css-values-3/lengths

    New in version 1.18.0: Only available with cairo 1.15.10+.
    """
    USER: "SVGUnit" = ...
    EM: "SVGUnit" = ...
    EX: "SVGUnit" = ...
    PX: "SVGUnit" = ...
    IN: "SVGUnit" = ...
    CM: "SVGUnit" = ...
    MM: "SVGUnit" = ...
    PT: "SVGUnit" = ...
    PC: "SVGUnit" = ...
    PERCENT: "SVGUnit" = ...

class PDFMetadata(_IntEnum):
    """
    `PDFMetadata` is used by the `PDFSurface.set_metadata()` method to specify the metadata to set.

    New in version 1.18.0: Only available with cairo 1.15.10+.
    """
    TITLE: "PDFMetadata" = ...
    AUTHOR: "PDFMetadata" = ...
    SUBJECT: "PDFMetadata" = ...
    KEYWORDS: "PDFMetadata" = ...
    CREATOR: "PDFMetadata" = ...
    CREATE_DATE: "PDFMetadata" = ...
    MOD_DATE: "PDFMetadata" = ...

class PDFOutlineFlags(_IntEnum):
    """
    `PDFOutlineFlags` is used by the `PDFSurface.add_outline()` method to specify the attributes of an outline item. These flags may be bitwise-or’d to produce any combination of flags.

    New in version 1.18.0: Only available with cairo 1.15.10+.
    """
    OPEN: "PDFOutlineFlags" = ...
    BOLD: "PDFOutlineFlags" = ...
    ITALIC: "PDFOutlineFlags" = ...

class ScriptMode(_IntEnum):
    """
    A set of script output variants.

    New in version 1.14.
    """
    ASCII: "ScriptMode" = ...
    BINARY: "ScriptMode" = ...

class Matrix:
    """
    Matrix is used throughout cairo to convert between different coordinate spaces. A Matrix holds an affine transformation, such as a scale, rotation, shear, or a combination of these. The transformation of a point (x,y) is given by:

    >>> x_new = xx * x + xy * y + x0
    >>> y_new = yx * x + yy * y + y0

    The current transformation matrix of a `Context`, represented as a Matrix, defines the transformation from user-space coordinates to device-space coordinates.

    Some standard Python operators can be used with matrices:

    To read the values from a Matrix:
    >>> xx, yx, xy, yy, x0, y0 = matrix\n

    To multiply two matrices:
    >>> matrix3 = matrix1.multiply(matrix2)
    >>> # or equivalently
    >>> matrix3 = matrix1 * matrix2\n

    To compare two matrices:
    >>> matrix1 == matrix2
    >>> matrix1 != matrix2\n

    For more information on matrix transformation see https://www.cairographics.org/cookbook/matrix_transform/
    """
    def __init__(self, xx: float = 1.0, yx: float = 0.0, yy: float = 1.0, x0: float = 0.0, y0: float = 0.0) -> None:
        """
        Create a new Matrix with the affine transformation given by xx, yx, xy, yy, x0, y0. The transformation is given by:
        >>> x_new = xx * x + xy * y + x0
        >>> y_new = yx * x + yy * y + y0

        To create a new identity matrix:
        >>> matrix = cairo.Matrix()

        To create a matrix with a transformation which translates by tx and ty in the X and Y dimensions, respectively:
        >>> matrix = cairo.Matrix(x0=tx, y0=ty)

        To create a matrix with a transformation that scales by sx and sy in the X and Y dimensions, respectively:
        >>> matrix = cairo.Matrix(xx=sy, yy=sy)
        """
    @classmethod
    def init_rotate(cls, radians: float) -> "Matrix":
        """
        Parameters
        ----------
        >>> radians (float)\n
        Angle of rotation, in radians. The direction of rotation is defined such that positive angles rotate in the direction from the positive X axis toward the positive Y axis. With the default axis orientation of cairo, positive angles rotate in a clockwise direction.
        
        Returns
        -------
        >>> cairo.Matrix\n
        A new `Matrix` set to a transformation that rotates by radians.
        """
    def invert(self) -> Optional["Matrix"]:
        """
        Changes `Matrix` to be the inverse of it’s original value. Not all transformation matrices have inverses; if the matrix collapses points together (it is degenerate), then it has no inverse and this function will fail.

        Returns
        -------
        >>> cairo.Matrix\n
        If `Matrix` has an inverse, modifies `Matrix` to be the inverse matrix and returns `None`
        
        Raises
        ------
        >>> cairo.Error\n
        If the `Matrix` has no inverse.
        """
    def multiply(self, matrix2: "Matrix") -> "Matrix":
        """
        Multiplies the affine transformations in Matrix and matrix2 together. The effect of the resulting transformation is to first apply the transformation in Matrix to the coordinates and then apply the transformation in matrix2 to the coordinates.

        It is allowable for result to be identical to either Matrix or matrix2.

        Parameters
        ----------
        >>> matrix2 (cairo.Matrix)\n
        A second `Matrix`
        
        Returns
        -------
        >>> cairo.Matrix\n
        A new `Matrix`
        """
    def rotate(self, radians: float) -> None:
        """
        Initialize `Matrix` to a transformation that rotates by radians.

        Parameters
        ----------
        >>> radians (float)\n
        Angle of rotation, in radians. The direction of rotation is defined such that positive angles rotate in the direction from the positive X axis toward the positive Y axis. With the default axis orientation of cairo, positive angles rotate in a clockwise direction.
        """
    def scale(self, sx: float, sy: float) -> None:
        """
        Applies scaling by sx, sy to the transformation in `Matrix`. The effect of the new transformation is to first scale the coordinates by sx and sy, then apply the original transformation to the coordinates.

        Parameters
        ----------
        >>> sx (float)\n
        Scale factor in the X direction.
        >>> sy (float)\n
        Scale factor in the Y direction.
        """
    def transform_distance(self, dx: float, dy: float) -> Tuple[float, float]:
        """
        Transforms the distance vector (dx,dy) by `Matrix`. This is similar to  `transform_point()` except that the translation components of the transformation are ignored. The calculation of the returned vector is as follows:
        >>> dx2 = dx1 * a + dy1 * c
        >>> dy2 = dx1 * b + dy1 * d

        Affine transformations are position invariant, so the same vector always transforms to the same vector. If (x1,y1) transforms to (x2,y2) then (x1+dx1,y1+dy1) will transform to (x1+dx2,y1+dy2) for all values of x1 and x2.

        Parameters
        ----------
        >>> dx (float)\n
        X component of a distance vector.
        >>> dy (float)\n
        Y component of a distance vector.

        Returns
        -------
        >>> (dx: float, dy: float)\n
        The transformed distance vector.
        """
    def transform_point(self, x: float, y: float) -> Tuple[float, float]:
        """
        Transforms the point (x, y) by Matrix.

        Parameters
        ----------
        >>> x (float)\n
        X position.
        >>> y (float)\n
        Y position.

        Returns
        -------
        >>> (x: float, y: float)\n
        The transformed point.
        """
    def translate(self, tx: float, ty: float) -> None:
        """
        Applies a transformation by tx, ty to the transformation in Matrix. The effect of the new transformation is to first translate the coordinates by tx and ty, then apply the original transformation to the coordinates.

        Parameters
        ----------
        >>> tx (float)\n
        Amount to translate in the X direction.
        >>> ty (float)\n
        Amount to translate in the Y direction.
        """
    xx: float = ...
    yx: float = ...
    xy: float = ...
    yy: float = ...
    x0: float = ...
    y0: float = ...

class Pattern:
    """
    Pattern is the abstract base class from which all the other pattern classes derive. It cannot be instantiated directly.
    """
    def get_extend(self) -> Extend:
        """
        Gets the current extend mode for the Pattern. See `cairo.Extend` attributes for details on the semantics of each extend strategy.

        Returns
        -------
        >>> cairo.Extend\n
        The current extend strategy used for drawing the `Pattern`.
        """
    def get_matrix(self) -> Matrix:
        """
        Returns
        -------
        >>> cairo.Matrix\n
        A new `Matrix` which stores a copy of the `Pattern`'s transformation matrix.
        """
    def get_filter(self) -> Filter:
        """
        New in version 1.12.0: Used to be a method of `SurfacePattern` before.

        Returns
        -------
        >>> cairo.Filter\n
        The current filter used for resizing the pattern.
        """
    def set_filter(self, filter: Filter) -> None:
        """
        Note that you might want to control filtering even when you do not have an explicit `Pattern` object, (for example when using `Context.set_source_surface()`). In these cases, it is convenient to use `Context.get_source()` to get access to the pattern that cairo creates implicitly. For example:
        >>> context.set_source_surface(image, x, y)
        >>> context.get_source().set_filter(cairo.FILTER_NEAREST)

        New in version 1.12.0: Used to be a method of `SurfacePattern` before.

        Parameters
        ----------
        >>> filter (cairo.Filter)\n
        A filter describing the filter to use for resizing the pattern.
        """
    def set_extend(self, extend: Extend) -> None:
        """
        Sets the mode to be used for drawing outside the area of a Pattern.

        The default extend mode is `cairo.Extend.NONE` for `SurfacePattern` and `cairo.Extend.PAD` for `Gradient` Patterns.

        Parameters
        ----------
        >>> extend (cairo.Extend)\n
        An extend describing how the area outside the `Pattern` will be drawn.
        """
    def set_matrix(self, matrix: Matrix) -> None:
        """
        Sets the Pattern’s transformation matrix to matrix. This matrix is a transformation from user space to pattern space.

        When a Pattern is first created it always has the identity matrix for its transformation matrix, which means that pattern space is initially identical to user space.

        Important: Please note that the direction of this transformation matrix is from user space to pattern space. This means that if you imagine the flow from a Pattern to user space (and on to device space), then coordinates in that flow will be transformed by the inverse of the Pattern matrix.

        For example, if you want to make a Pattern appear twice as large as it does by default the correct code to use is:
        >>> matrix = cairo.Matrix(xx=0.5,yy=0.5)
        >>> pattern.set_matrix(matrix)

        Meanwhile, using values of 2.0 rather than 0.5 in the code above would cause the Pattern to appear at half of its default size.

        Also, please note the discussion of the user-space locking semantics of `Context.set_source`.

        Parameters
        ----------
        >>> matrix (Matrix)
        """

class Glyph(Tuple[int, float, float]):
    """
    New in version 1.15: In prior versions a (int, float, float) tuple was used instead of `Glyph`.

    The `Glyph` holds information about a single glyph when drawing or measuring text. A font is (in simple terms) a collection of shapes used to draw text. A glyph is one of these shapes. There can be multiple glyphs for a single character (alternates to be used in different contexts, for example), or a glyph can be a ligature of multiple characters. Cairo doesn’t expose any way of converting input text into glyphs, so in order to use the Cairo interfaces that take arrays of glyphs, you must directly access the appropriate underlying font system.

    Note that the offsets given by x and y are not cumulative. When drawing or measuring text, each glyph is individually positioned with respect to the overall origin

    Parameters
    ----------
    >>> index (int)\n
    Glyph index in the font. The exact interpretation of the glyph index depends on the font technology being used.
    >>> x (float)\n
    The offset in the X direction between the origin used for drawing or measuring the string and the origin of this glyph.
    >>> y (float)\n
    The offset in the Y direction between the origin used for drawing or measuring the string and the origin of this glyph.

    Returns
    -------
    >>> cairo.Glyph\n
    A newly created `Glyph` instance.
    """
    index: int = ... # type: ignore
    x: float = ...
    y: float = ...
    def __init__(self, index: int, x: float, y: float) -> None: ...

class TextCluster(Tuple[int, int]):
    """
    New in version 1.15.

    The `TextCluster` structure holds information about a single text cluster. A text cluster is a minimal mapping of some glyphs corresponding to some UTF-8 text.

    For a cluster to be valid, both `num_bytes` and `num_glyphs` should be non-negative, and at least one should be non-zero. Note that clusters with zero glyphs are not as well supported as normal clusters. For example, PDF rendering applications typically ignore those clusters when PDF text is being selected.

    See `Context.show_text_glyphs()` for how clusters are used in advanced text operations.

    Parameters
    ----------
    >>> num_bytes (int)\n
    The number of bytes of UTF-8 text covered by cluster.
    >>> num_glyphs (int)\n
    The number of glyphs covered by cluster.
    """
    num_bytes: int = ...
    num_glyphs: int = ...
    def __init__(self, num_bytes: int, num_glyphs: int) -> None: ...

class TextClusterFlags(_IntEnum):
    """
    Specifies properties of a text cluster mapping.

    New in version 1.14.
    """
    BACKWARD: "TextClusterFlags" = ...

class TextExtents(Tuple[float, float, float, float, float, float]):
    """
    New in version 1.15: In prior versions a (float, float, float, float, float, float) tuple was used instead of `TextExtents`.

    The `TextExtents` class stores the extents of a single glyph or a string of glyphs in user-space coordinates. Because text extents are in user-space coordinates, they are mostly, but not entirely, independent of the current transformation matrix. If you call `context.scale(2.0, 2.0)`, text will be drawn twice as big, but the reported text extents will not be doubled. They will change slightly due to hinting (so you can’t assume that metrics are independent of the transformation matrix), but otherwise will remain unchanged.

    Parameters
    ----------
    >>> x_bearing (float)\n
    The horizontal distance from the origin to the leftmost part of the glyphs as drawn. Positive if the glyphs lie entirely to the right of the origin.
    >>> y_bearing (float)\n
    The vertical distance from the origin to the topmost part of the glyphs as drawn. Positive only if the glyphs lie completely below the origin; will usually be negative.
    >>> width (float)\n
    Width of the glyphs as drawn.
    >>> height (float)\n
    Height of the glyphs as drawn.
    >>> x_advance (float)\n
    Distance to advance in the X direction after drawing these glyphs.
    >>> y_advance (float)\n
    Distance to advance in the Y direction after drawing these glyphs. Will typically be zero except for vertical text layout as found in East-Asian languages.

    Returns
    -------
    >>> cairo.TextExtents\n
    A newly created `TextExtents` instance.
    """
    x_bearing: float = ...
    y_bearing: float = ...
    width: float = ...
    height: float = ...
    x_advance: float = ...
    y_advance: float = ...
    def __init__(self, x_bearing: float, y_bearing: float, width: float, height: float, x_advance: float, y_advance: float) -> None: ...

class RectangleInt:
    """
    RectangleInt is a data structure for holding a rectangle with integer coordinates.

    Allocates a new RectangleInt object.

    .. versionadded:: 1.11.0
    """

    height: int = ...
    width: int = ...
    x: int = ...
    y: int = ...

    def __init__(self, x: int=0, y: int=0, width: int=0, height: int=0) -> None:
        """
        :param x:
            X coordinate of the left side of the rectangle.
        :param y:
            Y coordinate of the top side of the rectangle.
        :param width:
            Width of the rectangle.
        :param height:
            Height of the rectangle.
        """


class FontFace:
    """
    A `cairo.FontFace` specifies all aspects of a font other than the size or font matrix (a font matrix is used to distort a font by sheering it or scaling it unequally in the two directions). A FontFace can be set on a `Context` by using `Context.set_font_face()` the size and font matrix are set with `Context.set_font_size()` and `Context.set_font_matrix()`.

    There are various types of FontFace, depending on the font backend they use.

    Note
    ----
    This class cannot be instantiated directly, it is returned by `Context.get_font_face()`.
    """

class FontOptions:
    """
    Allocates a new FontOptions object with all options initialized to default values.

    Implements `__eq__` and `__ne__` using `equal()` since 1.12.0.

    An opaque structure holding all options that are used when rendering fonts.

    Individual features of a FontOptions can be set or accessed using functions named FontOptions.set_<feature_name> and FontOptions.get_<feature_name>, like `FontOptions.set_antialias()` and `FontOptions.get_antialias()`.

    New features may be added to a `FontOptions` in the future. For this reason, `FontOptions.copy()`, `FontOptions.equal()`, `FontOptions.merge()`, and `FontOptions.hash()` should be used to copy, check for equality, merge, or compute a hash value of `FontOptions` objects.

    Returns
    -------
    >>> cairo.FontOptions\n
    A newly allocated FontOptions.
    """
    def get_antialias(self) -> Antialias:
        """
        Returns
        -------
        >>> cairo.Antialias\n
        The antialias mode for the `FontOptions` object.
        """
    def get_hint_style(self) -> HintStyle:
        """
        Returns
        -------
        >>> cairo.HintStyle\n
        The hint style for the `FontOptions` object.
        """
    def get_subpixel_order(self) -> SubpixelOrder:
        """
        Returns
        -------
        >>> cairo.SubpixelOrder\n
        The subpixel order for the `FontOptions` object.
        """
    def set_antialias(self, antialias: Antialias) -> None:
        """
        This specifies the ype of antialiasing to do when rendering text.

        Parameters
        ----------
        >>> antialias (cairo.Antialias)\n
        The antialias mode.
        """
    def set_hint_metrics(self, hint_metrics: HintMetrics) -> None:
        """
        This controls whether metrics are quantized to integer values in device units.

        Parameters
        ----------
        >>> hint_metrics (cairo.HintMetrics)\n
        The hint metrics mode.
        """
    def set_hint_style(self, hint_style: HintStyle) -> None:
        """
        This controls whether to fit font outlines to the pixel grid, and if so, whether to optimize for fidelity or contrast.

        Parameters
        ----------
        >>> hint_style (cairo.HintStyle)\n
        The hint style.
        """
    def merge(self, other: "FontOptions") -> None:
        """
        Merges non-default options from other into options , replacing existing values. This operation can be thought of as somewhat similar to compositing other onto options with the operation of `Operator.OVER.`

        New in version 1.12.0.

        Parameters
        ----------
        >>> other (cairo.FontOptions)\n
        Another `FontOptions`
        """
    def copy(self) -> "FontOptions":
        """
        Returns a new font options object copying the option values from original.

        New in version 1.12.0.

        Returns
        -------
        >>> cairo.FontOptions
        """
    def hash(self) -> int:
        """
        Compute a hash for the font options object; this value will be useful when storing an object containing a `FontOptions` in a hash table.

        New in version 1.12.0.

        Returns
        -------
        >>> int\n
        The hash value for the font options object.
        """
    def equal(self, other: "FontOptions") -> bool:
        """
        Compares two font options objects for equality.

        New in version 1.12.0.

        Parameters
        ----------
        >>> other (cairo.FontOptions)\n
        Another `FontOptions`

        Returns
        -------
        >>> bool\n
        `True` if all fields of the two font options objects match. Note that this function will return `False` if either object is in error.
        """
    def set_variations(self, variations: Optional[str]) -> None:
        """
        Sets the OpenType font variations for the font options object. Font variations are specified as a string with a format that is similar to the CSS font-variation-settings. The string contains a comma-separated list of axis assignments, which each assignment consists of a 4-character axis name and a value, separated by whitespace and optional equals sign.

        Examples:
        - wght=200,wdth=140.5
        - wght 200 , wdth 140.5

        New in version 1.18.0: Only available with cairo 1.15.12+

        Parameters
        ----------
        >>> variations (str or None)\n
        The new font variations, or `None`.
        """
    def get_variations(self) -> str:
        """
        Gets the OpenType font variations for the font options object. See `set_variations()` for details about the string format.

        New in version 1.18.0: Only available with cairo 1.15.12+

        Returns
        -------
        >>> str\n
        The font variations for the font options object. The returned string belongs to the options and must not be modified. It is valid until either the font options object is destroyed or the font variations in this object is modified with `set_variations()`.
        """
    def get_hint_metrics(self) -> HintMetrics: ...
    def set_subpixel_order(self, subpixel_order: SubpixelOrder) -> None: ...

class ScaledFont:
    """
    Creates a ScaledFont object from a FontFace and matrices that describe the size of the font and the environment in which it will be used.

    A ScaledFont is a font scaled to a particular size and device resolution. A ScaledFont is most useful for low-level font usage where a library or application wants to cache a reference to a scaled font to speed up the computation of metrics.

    There are various types of scaled fonts, depending on the font backend they use.

    Parameters
    ----------
    >>> font_face (cairo.FontFace)\n
    A `FontFace` instance.
    >>> font_matrix (cairo.Matrix)\n
    Font space to user space transformation `Matrix` for the font. In the simplest case of a N point font, this matrix is just a scale by N, but it can also be used to shear the font or stretch it unequally along the two axes. See `Context.set_font_matrix()`.
    >>> ctm (cairo.Matrix)\n
    User to device transformation `Matrix` with which the font will be used.
    >>> options (cairo.FontOptions)\n
    A `FontOptions` instance to use when getting metrics for the font and rendering with it.
    """
    def __init__(self, font_face: FontFace, font_matrix: Matrix, ctm: Matrix, options: FontOptions) -> None: ...
    def extents(self) -> Tuple[float, float, float, float, float]:
        """
        Gets the metrics for a ScaledFont.

        Returns
        -------
        >>> (ascent: float, descent: float, height: float, max_x_advance: float, max_y_advance: float)
        """
    def get_ctm(self) -> Matrix:
        """
        Returns the CTM with which scaled_font was created into ctm. Note that the translation offsets (x0, y0) of the CTM are ignored by `ScaledFont()`. So, the matrix this function returns always has 0, 0 as x0, y0.

        New in version 1.12.0.

        Returns
        -------
        >>> cairo.Matrix\n
        The CTM.
        """
    def get_font_face(self) -> FontFace:
        """
        New in version 1.2.

        Returns
        -------
        >>> cairo.FontFace\n
        The `FontFace` that this `ScaledFont` was created for.
        """
    def get_font_matrix(self) -> Matrix:
        """
        Returns the font matrix with which scaled_font was created.

        New in version 1.12.0.

        Returns
        -------
        >>> cairo.Matrix\n
        The Matrix.
        """
    def get_font_options(self) -> FontOptions:
        """
        Returns the font options with which scaled_font was created.

        New in version 1.12.0.

        Returns
        -------
        >>> cairo.FontOptions\n
        Font options.
        """
    def get_scale_matrix(self) -> Matrix:
        """
        The scale matrix is product of the font matrix and the ctm associated with the scaled font, and hence is the matrix mapping from font space to device space.

        New in version 1.8.

        Returns
        -------
        >>> cairo.Matrix\n
        The scale `Matrix`.
        """
    def glyph_extents(self, glyphs: Sequence[Glyph]) -> TextExtents:
        """
        New in version 1.15.

        Gets the extents for a list of glyphs. The extents describe a user-space rectangle that encloses the “inked” portion of the glyphs, (as they would be drawn by `Context.show_glyphs()` if the cairo graphics state were set to the same font_face, font_matrix, ctm, and font_options as scaled_font ). Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by `cairo_show_glyphs()`.

        Note that whitespace glyphs do not contribute to the size of the rectangle (extents.width and extents.height).

        Parameters
        ----------
        >>> glyphs list(cairo.Glyph)\n
        A sequence of `Glyph`.

        Returns
        -------
        >>> cairo.TextExtents\n
        """
    def text_extents(self, text: str) -> TextExtents:
        """
        Gets the extents for a string of text. The extents describe a user-space rectangle that encloses the “inked” portion of the text drawn at the origin (0,0) (as it would be drawn by `Context.show_text()` if the cairo graphics state were set to the same font_face, font_matrix, ctm, and font_options as `ScaledFont`). Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by `Context.show_text()`.

        Note that whitespace characters do not directly contribute to the size of the rectangle (width and height). They do contribute indirectly by changing the position of non-whitespace characters. In particular, trailing whitespace characters are likely to not affect the size of the rectangle, though they will affect the x_advance and y_advance values.

        Parameters
        ----------
        >>> text (str)

        Returns
        -------
        >>> cairo.TextExtents
        """
    def text_to_glyphs(self, x: float, y: float, utf8: str, with_clusters: bool = True) -> Union[Tuple[List[Glyph], List["TextCluster"], TextClusterFlags], List[Glyph]]:
        """
        New in version 1.15.

        Converts UTF-8 text to a list of glyphs, with cluster mapping, that can be used to render later.

        For details of how clusters, and cluster_flags map input UTF-8 text to the output glyphs see `Context.show_text_glyphs()`.

        The output values can be readily passed to `Context.show_text_glyphs()` `Context.show_glyphs()`, or related functions, assuming that the exact same scaled font is used for the operation.

        Parameters
        ----------
        >>> x (float)\n
        X position to place first glyph.
        >>> y (float)\n
        Y position to place first glyph.
        >>> utf8 (str)\n
        A string of text.
        >>> with_clusters (bool)\n
        If `False` only the glyph list will be computed and returned.

        Returns
        -------
        >>> ([Glyph], [TextCluster], TextClusterFlags) or [Glyph]

        Raises
        ------
        >>> cairo.Error\n
        """

_SomeDevice = TypeVar("_SomeDevice", bound="Device")

class Device:
    """
    A Device represents the driver interface for drawing operations to a Surface.

    New in version 1.14.

    Note
    ----
    New in version 1.17.0: `cairo.Device` can be used as a context manager:
    >>> # device.finish() will be called on __exit__
    >>> with cairo.ScriptDevice(f) as device:
    >>> \tpass
    """
    def finish(self) -> None:
        """
        This function finishes the device and drops all references to external resources. All surfaces, fonts and other objects created for this device will be finished, too. Further operations on the device will not affect the device but will instead trigger a Status.DEVICE_FINISHED error.

        This function may acquire devices.

        New in version 1.14.
        """
    def flush(self) -> None:
        """
        Finish any pending operations for the device and also restore any temporary modifications cairo has made to the device’s state. This function must be called before switching from using the device with Cairo to operating on it directly with native APIs. If the device doesn’t support direct access, then this function does nothing.

        This function may acquire devices.

        New in version 1.14.
        """
    def acquire(self) -> None:
        """
        Acquires the device for the current thread. This function will block until no other thread has acquired the device.

        If the does not raise, you successfully acquired the device. From now on your thread owns the device and no other thread will be able to acquire it until a matching call to `release()`. It is allowed to recursively acquire the device multiple times from the same thread.

        After a successful call to `acquire()`, a matching call to `release()` is required.

        New in version 1.14.

        Note
        ----
        You must never acquire two different devices at the same time unless this is explicitly allowed. Otherwise the possibility of deadlocks exist. As various Cairo functions can acquire devices when called, these functions may also cause deadlocks when you call them with an acquired device. So you must not have a device acquired when calling them. These functions are marked in the documentation.

        Raises
        ------
        >>> cairo.Error\n
        If the device is in an error state and could not be acquired.
        """
    def release(self) -> None:
        """
        Releases a device previously acquired using `acquire()`. See that function for details.

        New in version 1.14.
        """
    def __enter__(self: _SomeDevice) -> _SomeDevice: ...
    __exit__: Any = ...

_PathLike = Union[Text, ByteString]
_FileLike = BinaryIO
_SomeSurface = TypeVar("_SomeSurface", bound="Surface")

class Surface:
    """
    Surface is the abstract base class from which all the other surface classes derive. It cannot be instantiated directly.

    Note
    ----
    New in version 1.17.0: `cairo.Surface` can be used as a context manager:
    >>> # surface.finish() will be called on __exit__
    >>> with cairo.SVGSurface("example.svg", 200, 200) as surface:
    >>>     pass
    >>> 
    >>> # surface.unmap_image(image_surface) will be called on __exit__
    >>> with surface.map_to_image(None) as image_surface:
    >>>     pass
    """
    def copy_page(self) -> None:
        """
        Emits the current page for backends that support multiple pages, but doesn’t clear it, so that the contents of the current page will be retained for the next page. Use `show_page()` if you want to get an empty page after the emission.

        `Context.copy_page()` is a convenience function for this.

        New in version 1.6.
        """
    def create_for_rectangle(self, x: float, y: float, width: float, height: float) -> "Surface":
        """
        Create a new surface that is a rectangle within the target surface. All operations drawn to this surface are then clipped and translated onto the target surface. Nothing drawn via this sub-surface outside of its bounds is drawn onto the target surface, making this a useful method for passing constrained child surfaces to library routines that draw directly onto the parent surface, i.e. with no further backend allocations, double buffering or copies.

        New in version 1.12.0.

        Note
        ----
        The semantics of subsurfaces have not been finalized yet unless the rectangle is in full device units, is contained within the extents of the target surface, and the target or subsurface’s device transforms are not changed.

        Parameters
        ----------
        >>> x (float)\n
        The x-origin of the sub-surface from the top-left of the target surface (in device-space units).
        >>> y (float)\n
        The y-origin of the sub-surface from the topleft of the target surface (in device-space units).
        >>> width (float)\n
        Width of the sub-surface (in device-space units).
        >>> height (float)\n
        Height of the sub-surface (in device-space units).

        Returns
        -------
        >>> cairo.Surface\n
        A new surface.
        """
    def create_similar(self, content: Content, width: int, height: int) -> "Surface":
        """
        Create a `Surface` that is as compatible as possible with the existing surface. For example the new surface will have the same fallback resolution and `FontOptions`. Generally, the new surface will also use the same backend, unless that is not possible for some reason.

        Initially the surface contents are all 0 (transparent if contents have transparency, black otherwise.)

        Parameters
        ----------
        >>> content (cairo.Content)\n
        The content for the new surface.
        >>> width (int)\n
        Width of the new surface, in device-space units.
        >>> height (int)\n
        Height of the new surface, in device-space units.

        Returns
        -------
        >>> cairo.Surface\n
        A newly allocated `Surface`.
        """
    def create_similar_image(self, format: Format, width: int, height: int) -> "ImageSurface":
        """
        Create a new image surface that is as compatible as possible for uploading to and the use in conjunction with an existing surface. However, this surface can still be used like any normal image surface.

        Initially the surface contents are all 0 (transparent if contents have transparency, black otherwise.)

        New in version 1.12.0.

        Parameters
        ----------
        >>> format (cairo.Format)\n
        The format for the new surface.
        >>> width (int)\n
        Width of the new surface, in device-space units.
        >>> height (int)\n
        Height of the new surface, in device-space units.
        """
    def finish(self) -> None:
        """
        This method finishes the Surface and drops all references to external resources. For example, for the Xlib backend it means that cairo will no longer access the drawable, which can be freed. After calling finish() the only valid operations on a Surface are flushing and finishing it. Further drawing to the surface will not affect the surface but will instead trigger a `cairo.Error` exception.
        """
    def flush(self) -> None:
        """
        Do any pending drawing for the Surface and also restore any temporary modification’s cairo has made to the Surface’s state. This method must be called before switching from drawing on the Surface with cairo to drawing on it directly with native APIs. If the Surface doesn’t support direct access, then this function does nothing.
        """
    def get_content(self) -> Content:
        """
        New in version 1.2.

        Returns
        -------
        >>> cairo.Content\n
        The content type of `Surface`, which indicates whether the `Surface` contains color and/or alpha information.
        """
    def get_device(self) -> Optional["Device"]:
        """
        This function returns the device for a surface.

        New in version 1.14.0.

        Returns
        -------
        >>> cairo.Device or None\n
        The device or `None` if the surface does not have an associated device.
        """
    def get_device_offset(self) -> Tuple[float, float]:
        """
        This method returns the previous device offset set by `set_device_offset()`.

        New in version 1.2.

        Returns
        -------
        >>> (x_offset: float, y_offset: float)\n
        - `x_offset (float)` -- The offset in the X direction, in device units.
        - `y_offset (float)` -- The offset in the Y direction, in device units.
        """
    def get_device_scale(self) -> Tuple[float, float]:
        """
        This function returns the previous device offset set by `Surface.set_device_scale()`.

        New in version 1.14.0.
        
        Returns
        -------
        >>> (x_scale: float, y_scale: float)\n
        """
    def get_fallback_resolution(self) -> Tuple[float, float]:
        """
        This method returns the previous fallback resolution set by `set_fallback_resolution()`, or default fallback resolution if never set.

        New in version 1.8.

        Returns
        -------
        >>> (x_pixels_per_inch: float, y_pixels_per_inch: float)\n
        - `x_pixels_per_inch (float)` -- Horizontal pixels per inch.
        - `y_pixels_per_inch (float)` -- Vertical pixels per inch.
        """
    def get_font_options(self) -> FontOptions:
        """
        Retrieves the default font rendering options for the Surface. This allows display surfaces to report the correct subpixel order for rendering on them, print surfaces to disable hinting of metrics and so forth. The result can then be used with `ScaledFont`.

        Returns
        -------
        >>> cairo.FontOptions\n
        """
    def get_mime_data(self, mime_type: str) -> Optional[bytes]:
        """
        Return mime data previously attached to surface with `set_mime_data()` using the specified mime type. If no data has been attached with the given mime type, `None` is returned.

        New in version 1.12.0.

        Parameters
        ----------
        >>> mime_type (str)\n
        The MIME type of the image data (`cairo.MIME_TYPE`).

        Returns
        -------
        >>> bytes or None\n
        """
    def has_show_text_glyphs(self) -> bool:
        """
        Returns whether the surface supports sophisticated `Context.show_text_glyphs()` operations. That is, whether it actually uses the provided text and cluster data to a `Context.show_text_glyphs()` call.

        Note: Even if this function returns `False`, a `Context.show_text_glyphs()` operation targeted at surface will still succeed. It just will act like a `Context.show_glyphs()` operation. Users can use this function to avoid computing UTF-8 text and cluster mapping if the target surface does not use it.

        New in version 1.12.0.

        Returns
        -------
        >>> bool\n
        `True` if surface supports `Context.show_text_glyphs()`, `False` otherwise.
        """
    def map_to_image(self, extents: Optional[RectangleInt]) -> "ImageSurface":
        """
        Returns an image surface that is the most efficient mechanism for modifying the backing store of the target surface.

        Note, the use of the original surface as a target or source whilst it is mapped is undefined. The result of mapping the surface multiple times is undefined. Calling `Surface.finish()` on the resulting image surface results in undefined behavior. Changing the device transform of the image surface or of surface before the image surface is unmapped results in undefined behavior.

        The caller must use `Surface.unmap_image()` to destroy this image surface.

        New in version 1.15.0.

        Parameters
        ----------
        >>> extents (cairo.RectangleInt or None)\n
        Limit the extraction to an rectangular region, or `None` for the whole surface.

        Returns
        -------
        >>> cairo.ImageSurface\n
        Newly allocated image surface.

        Raises
        ------
        >>> cairo.Error\n
        """
    def mark_dirty(self) -> None:
        """
        Tells cairo that drawing has been done to Surface using means other than cairo, and that cairo should reread any cached areas. Note that you must call `flush()` before doing such drawing.
        """
    def mark_dirty_rectangle(self, x: int, y: int, width: int, height: int) -> None:
        """
        Like `mark_dirty()`, but drawing has been done only to the specified rectangle, so that cairo can retain cached contents for other parts of the surface.

        Any cached clip set on the Surface will be reset by this function, to make sure that future cairo calls have the clip set that they expect.

        Parameters
        ----------
        >>> x (int)\n
        X coordinate of dirty rectangle.
        >>> y (int)\n
        Y coordinate of dirty rectangle.
        >>> width (int)\n
        Width of dirty rectangle.
        >>> height (int)\n
        Height of dirty rectangle.
        """
    def set_device_offset(self, x_offset: float, y_offset: float) -> None:
        """
        Sets an offset that is added to the device coordinates determined by the CTM when drawing to Surface. One use case for this function is when we want to create a Surface that redirects drawing for a portion of an onscreen surface to an offscreen surface in a way that is completely invisible to the user of the cairo API. Setting a transformation via `Context.translate()` isn’t sufficient to do this, since functions like `Context.device_to_user()` will expose the hidden offset.

        Note that the offset affects drawing to the surface as well as using the surface in a source pattern.

        Parameters
        ----------
        >>> x_offset (float)\n
        The offset in the X direction, in device units.
        >>> y_offset (float)\n
        The offset in the Y direction, in device units.
        """
    def set_device_scale(self, x_scale: float, y_scale: float) -> None:
        """
        Sets a scale that is multiplied to the device coordinates determined by the CTM when drawing to surface . One common use for this is to render to very high resolution display devices at a scale factor, so that code that assumes 1 pixel will be a certain size will still work. Setting a transformation via `Context.translate()` isn’t sufficient to do this, since functions like `Context.device_to_user()` will expose the hidden scale.

        New in version 1.14.0.

        Parameters
        ----------
        >>> x_scale (float)\n
        A scale factor in the X direction.
        >>> y_scale (float)\n
        A scale factor in the Y direction.
        """
    def set_fallback_resolution(self, x_pixels_per_inch: float, y_pixels_per_inch: float) -> None:
        """
        Set the horizontal and vertical resolution for image fallbacks.

        When certain operations aren’t supported natively by a backend, cairo will fallback by rendering operations to an image and then overlaying that image onto the output. For backends that are natively vector-oriented, this function can be used to set the resolution used for these image fallbacks, (larger values will result in more detailed images, but also larger file sizes).

        Some examples of natively vector-oriented backends are the ps, pdf, and svg backends.

        For backends that are natively raster-oriented, image fallbacks are still possible, but they are always performed at the native device resolution. So this function has no effect on those backends.

        Note: The fallback resolution only takes effect at the time of completing a page (with `Context.show_page()` or `Context.copy_page()`) so there is currently no way to have more than one fallback resolution in effect on a single page.

        The default fallback resoultion is 300 pixels per inch in both dimensions.

        New in version 1.2.

        Parameters
        ----------
        >>> x_pixels_per_inch (float)\n
        Horizontal setting for pixels per inch.
        >>> y_pixels_per_inch (float)\n
        Vertical setting for pixels per inch.
        """
    def set_mime_data(self, mime_type: str, data: bytes) -> None:
        """
        Attach an image in the format `mime_type` to Surface. To remove the data from a surface, call this function with same mime type and `None` for data.

        The attached image (or filename) data can later be used by backends which support it (currently: PDF, PS, SVG and Win32 Printing surfaces) to emit this data instead of making a snapshot of the surface. This approach tends to be faster and requires less memory and disk space.

        The recognized MIME types are listed under `cairo.MIME_TYPE`.

        See corresponding backend surface docs for details about which MIME types it can handle. Caution: the associated MIME data will be discarded if you draw on the surface afterwards. Use this function with care.

        New in version 1.12.0.

        Parameters
        ----------
        >>> mime_type (str)\n
        The MIME type of the image data (`cairo.MIME_TYPE`).
        >>> data (bytes)\n
        The image data to attach to the surface.
        """
    def show_page(self) -> None:
        """
        Emits and clears the current page for backends that support multiple pages. Use `copy_page()` if you don’t want to clear the page.

        There is a convenience function for this that takes a `Context.show_page()`.

        New in version 1.6.
        """
    def supports_mime_type(self, mime_type: str) -> bool:
        """
        Returns whether surface supports `mime_type`.

        New in version 1.12.0.

        Parameters
        ----------
        >>> mime_type (str)\n
        The mime type (`cairo.MIME_TYPE`).

        Returns
        -------
        >>> bool\n
        `True` if surface supports mime_type, `False` otherwise.
        """
    def write_to_png(self, fobj: Union[_FileLike, _PathLike]) -> None:
        """
        Writes the contents of `Surface` to fobj as a PNG image.

        Parameters
        ----------
        >>> fobj (str)\n
        (filename (`pathlike`), file or file-like object) - The file to write to.

        Raises
        ------
        >>> cairo.MemoryError\n
        If memory could not be allocated for the operation.
        >>> cairo.IOError\n
        If an I/O error occurs while attempting to write the file.
        """
    def unmap_image(self, image: "ImageSurface") -> None:
        """
        Unmaps the image surface as returned from `Surface.map_to_image()`.

        The content of the image will be uploaded to the target surface. Afterwards, the image is destroyed.

        Using an image surface which wasn’t returned by `Surface.map_to_image()` results in undefined behavior.

        New in version 1.15.0.

        Parameters
        ----------
        >>> image (cairo.ImageSurface)\n
        The currently mapped image.
        """
    def __enter__(self: _SomeSurface) -> _SomeSurface: ...
    __exit__: Any = ...

class ImageSurface(Surface):
    """
    Creates an ImageSurface of the specified format and dimensions. Initially the surface contents are all 0. (Specifically, within each pixel, each color or alpha channel belonging to format will be 0. The contents of bits within a pixel, but not belonging to the given format are undefined).

    Parameters
    ----------
    >>> format (cairo.Format)\n
    Format of pixels in the surface to create.
    >>> width (int)\n
    Width of the surface, in pixels.
    >>> height (int)\n
    Height of the surface, in pixels.

    Returns
    -------
    >>> cairo.ImageSurface\n
    A newly allocated `ImageSurface`.

    Raises
    ------
    >>> cairo.MemoryError\n
    In case of no memory.
    """
    def __init__(self, format: Format, width: int, height: int) -> None: ...
    @classmethod
    def create_for_data(cls, data: memoryview, format: Format, width: int, height: int, stride: int = ...) -> "ImageSurface":
        """
        Creates an ImageSurface for the provided pixel data. The initial contents of buffer will be used as the initial image contents; you must explicitly clear the buffer, using, for example, `cairo_rectangle()` and `cairo_fill()` if you want it cleared.

        Note that the stride may be larger than width*bytes_per_pixel to provide proper alignment for each pixel and row. This alignment is required to allow high-performance rendering within cairo. The correct way to obtain a legal stride value is to call `cairo.Format.stride_for_width()` with the desired format and maximum image width value, and use the resulting stride value to allocate the data and to create the ImageSurface. See `cairo.Format.stride_for_width()` for example code.

        Parameters
        ----------
        >>> data (buffer/memoryview)\n
        A writable Python buffer/memoryview object.
        >>> format (cairo.Format)\n
        The format of pixels in the buffer.
        >>> width (int)\n
        The width of the image to be stored in the buffer.
        >>> height (int)\n
        The height of the image to be stored in the buffer.
        >>> stride (int or None)\n
        The number of bytes between the start of rows in the buffer as allocated. If not given, the value from `cairo.Format.stride_for_width()` is used.

        Returns
        -------
        >>> cairo.ImageSurface\n
        A new `ImageSurface`.

        Raises
        ------
        >>> cairo.MemoryError\n
        In case of no memory.
        >>> cairo.Error\n
        In case of invalid stride value.
        """
    @classmethod
    def create_from_png(cls, fobj: Union[_PathLike, _FileLike]) -> "ImageSurface":
        """
        Parameters
        ----------
        >>> fobj (str)\n
        A `pathlike`, file or file-like object of the PNG to load.

        Returns
        -------
        >>> cairo.ImageSurface\n
        A new `ImageSurface` initialized from the contents of the given PNG file.
        """
    format_stride_for_width = Format.stride_for_width
    def get_data(self) -> memoryview:
        """
        New in version 1.2.

        Returns
        -------
        >>> memoryview\n
        A Python buffer object for the data of the `ImageSurface`, for direct inspection or modification. In Python 3 a memoryview object is returned.
        """
    def get_format(self) -> Format:
        """
        New in version 1.2.

        Returns
        -------
        >>> cairo.Format\n
        The format of the `ImageSurface`.
        """
    def get_height(self) -> int:
        """
        Returns
        -------
        >>> int\n
        The height of the `ImageSurface` in pixels.
        """
    def get_stride(self) -> bytes:
        """
        Returns
        -------
        >>> bytes\n
        The stride of the `ImageSurface` in bytes. The stride is the distance in bytes from the beginnning of one row of the image data to the beginning of the next row.
        """
    def get_width(self) -> int:
        """
        Returns
        -------
        >>> int\n
        The width of the `ImageSurface` in pixels.
        """

class SurfacePattern(Pattern):
    """
    Parameters
    ----------
    >>> surface (Surface)\n
    A cairo `Surface`.

    Returns
    -------
    >>> SurfacePattern\n
    A newly created `SurfacePattern` for the given surface.

    Raises
    ------
    >>> MemoryError\n
    In case of no memory.
    """
    def __init__(self, surface: Surface) -> None: ...
    def get_surface(self) -> Surface:
        """
        New in version 1.4.

        Returns
        -------
        >>> cairo.Surface\n
        The `Surface` of the `SurfacePattern`.
        """

class Context:
    """
    Creates a new Context with all graphics state parameters set to default values and with target as a target surface. The target surface should be constructed with a backend-specific function such as ImageSurface (or any other cairo backend surface create variant).

    Parameters
    ----------
    >>> target (cairo.Surface)\n
    Target `Surface` for the context.
    
    Returns
    -------
    >>> cairo.Context\n
    A newly allocated `Context`.
    
    Raises
    ------
    >>> cairo.MemoryError\n
    In case of no memory.
    """
    def __init__(self, target: Surface) -> None: ...
    def append_path(self, path: Path) -> None:
        """
        Append the path onto the current path. The path may be either the return value from one of Context.copy_path() or Context.copy_path_flat() or it may be constructed manually (in C).
        """
    def arc(self, xc: float, yc: float, radius: float, angle1: float, angle2: float) -> None:
        """
        Adds a circular arc of the given radius to the current path. The arc is centered at (xc, yc), begins at `angle1` and proceeds in the direction of increasing angles to end at `angle2`. If `angle2` is less than `angle1` it will be progressively increased by `2*PI` until it is greater than `angle1`.

        If there is a current point, an initial line segment will be added to the path to connect the current point to the beginning of the arc. If this initial line is undesired, it can be avoided by calling `Context.new_sub_path()` before calling `Context.arc()`.

        Angles are measured in radians. An angle of 0.0 is in the direction of the positive X axis (in user space). An angle of `PI/2.0 radians (90 degrees)` is in the direction of the positive Y axis (in user space). Angles increase in the direction from the positive X axis toward the positive Y axis. So with the default transformation matrix, angles increase in a clockwise direction.

        To convert from degrees to radians, use `degrees * (math.pi / 180)`.

        This function gives the arc in the direction of increasing angles; see `Context.arc_negative()` to get the arc in the direction of decreasing angles.

        The arc is circular in user space. To achieve an elliptical arc, you can scale the current transformation matrix by different amounts in the X and Y directions. For example, to draw an ellipse in the box given by x, y, width, height:
        >>> ctx.save()
        >>> ctx.translate(x + width / 2., y + height / 2.)
        >>> ctx.scale(width / 2., height / 2.)
        >>> ctx.arc(0., 0., 1., 0., 2 * math.pi)
        >>> ctx.restore()\n

        Parameters
        ----------
        >>> xc (float)\n
        X position of the center of the arc.
        >>> yc (float)\n
        Y position of the of the arc
        >>> radius (float)\n
        The radius of the arc.
        >>> angle1 (float)\n
        The start of the angle, in radians.
        >>> angle2 (float)\n
        The end of the angle, in radians.
        """
    def arc_negative(self, xc: float, yc: float, radius: float, angle1: float, angle2: float) -> None:
        """
        Adds a circular arc of the given radius to the current path. The arc is centered at (xc, yc), begins at angle1 and proceeds in the direction of decreasing angles to end at angle2. If angle2 is greater than angle1 it will be progressively decreased by 2*PI until it is less than angle1.

        See `Context.arc()` for more details. This function differs only in the direction of the arc between the two angles.
        
        Parameters
        ----------
        >>> xc (float)\n
        X position of the center of the arc.
        >>> yc (float)\n
        Y position of the center of the arc.
        >>> radius (float)\n
        The radius of the arc.
        >>> angle1 (float)\n
        The start angle, in radians.
        >>> angle2 (float)\n
        The end angle, in radians.
        """
    def clip(self) -> None:
        """
        Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by `Context.fill()` and according to the current `fill rule` (see `Context.set_fill_rule()`).

        After `clip()`, the current path will be cleared from the `Context`.

        The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.

        Calling `clip()` can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling `clip()` within a `Context.save()`/`Context.restore()` pair. The only other means of increasing the size of the clip region is `Context.reset_clip()`.
        """
    def clip_extents(self) -> Tuple[float, float, float, float]:
        """
        Computes a bounding box in user coordinates covering the area inside the current clip.

        New in version 1.4.

        Returns
        -------
        >>> (x1: float, y1: float, x2: float, y2: float)\n
        - `x1 (float)` -- Left of the resulting extents.
        - `y1 (float)` -- Top of the resulting extents.
        - `x2 (float)` -- Right of the resulting extents.
        - `y2 (float)` -- Bottom of the resulting extents.
        """
    def clip_preserve(self) -> None:
        """
        Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by `Context.fill()` and according to the current `fill rule` (see `Context.set_fill_rule()`).

        Unlike `Context.clip()`, `clip_preserve()` preserves the path within the `Context`.

        The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.

        Calling `clip_preserve()` can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling `clip_preserve()` within a `Context.save()`/`Context.restore()` pair. The only other means of increasing the size of the clip region is `Context.reset_clip()`.
        """
    def close_path(self) -> None:
        """
        Adds a line segment to the path from the current point to the beginning of the current sub-path, (the most recent point passed to `Context.move_to()`), and closes this sub-path. After this call the current point will be at the joined endpoint of the sub-path.

        The behavior of `close_path()` is distinct from simply calling `Context.line_to()` with the equivalent coordinate in the case of stroking. When a closed sub-path is stroked, there are no caps on the ends of the sub-path. Instead, there is a line join connecting the final and initial segments of the sub-path.

        If there is no current point before the call to `close_path()`, this function will have no effect.

        Note: As of cairo version 1.2.4 any call to `close_path()` will place an explicit MOVE_TO element into the path immediately after the CLOSE_PATH element, (which can be seen in `Context.copy_path()` for example). This can simplify path processing in some cases as it may not be necessary to save the “last move_to point” during processing as the MOVE_TO immediately after the CLOSE_PATH will provide that point.
        """
    def copy_clip_rectangle_list(self) -> List[Rectangle]:
        """
        New in version 1.4

        Returns
        -------
        >>> list(cairo.Rectangle)\n
        The current clip region as a list of rectangles in user coordinates. Returns a list of `Rectangle`
        """
    def copy_page(self) -> None:
        """
        Emits the current page for backends that support multiple pages, but doesn’t clear it, so, the contents of the current page will be retained for the next page too. Use `Context.show_page()` if you want to get an empty page after the emission.

        This is a convenience function that simply calls `Surface.copy_page()` on Context’s target.
        """
    def copy_path(self) -> Path:
        """
        Creates a copy of the current path and returns it to the user as a `Path`.

        Returns
        -------
        >>> cairo.Path\n
        
        Raises
        ------
        >>> cairo.MemoryError\n
        In case of no memory.
        """
    def copy_path_flat(self) -> Path:
        """
        Gets a flattened copy of the current path and returns it to the user as a `Path`.

        This function is like `Context.copy_path()` except that any curves in the path will be approximated with piecewise-linear approximations, (accurate to within the current tolerance value). That is, the result is guaranteed to not have any elements of type CAIRO_PATH_CURVE_TO which will instead be replaced by a series of CAIRO_PATH_LINE_TO elements.

        Returns
        -------
        >>> cairo.Path\n
        
        Raises
        ------
        >>> cairo.MemoryError\n
        In case of no memory.
        """
    def curve_to(self, x1: float, y1: float, x2: float, y2: float, x3: float, y3: float) -> None:
        """
        Adds a cubic Bézier spline to the path from the current point to position (x3, y3) in user-space coordinates, using (x1, y1) and (x2, y2) as the control points. After this call the current point will be (x3, y3).

        If there is no current point before the call to `curve_to()` this function will behave as if preceded by a call to `ctx.move_to(x1, y1)`.

        Parameters
        ----------
        >>> x1 (float)\n 
        The X coordinate of the first control point
        >>> y1 (float)\n 
        The Y coordinate of the first control point
        >>> x2 (float)\n 
        The X coordinate of the second control point
        >>> y2 (float)\n 
        The Y coordinate of the second control point
        >>> x3 (float)\n 
        The X coordinate of the end of the curve
        >>> y3 (float)\n 
        The Y coordinate of the end of the curve
        """
    def device_to_user(self, x: float, y: float) -> Tuple[float, float]:
        """
        Transform a coordinate from device space to user space by multiplying the given point by the inverse of the current transformation matrix (CTM).

        Parameters
        ----------
        >>> x (float)\n
        X value of coordinate
        >>> y (float)\n
        Y value of coordinate
        
        Returns
        -------
        >>> (x: float, y: float)\n
        """
    def device_to_user_distance(self, dx: float, dy: float) -> Tuple[float, float]:
        """
        Transform a distance vector from device space to user space. This function is similar to `Context.device_to_user()` except that the translation components of the inverse CTM will be ignored when transforming (dx,dy).

        Parameters
        ----------
        >>> dx (float)\n
        X component of a distance vector
        >>> dy (float)\n
        Y component of a distance vector
        
        Returns
        -------
        >>> (dx: float, dy: float)\n
        """
    def fill(self) -> None:
        """
        A drawing operator that fills the current path according to the current `fill rule`, (each sub-path is implicitly closed before being filled). After `fill()`, the current path will be cleared from the `Context`. See `Context.set_fill_rule()` and `Context.fill_preserve()`.
        """
    def fill_extents(self) -> Tuple[float, float, float, float]:
        """
        Computes a bounding box in user coordinates covering the area that would be affected, (the “inked” area), by a `Context.fill()` operation given the current path and fill parameters. If the current path is empty, returns an empty rectangle (0,0,0,0). Surface dimensions and clipping are not taken into account.

        Contrast with `Context.path_extents()`, which is similar, but returns non-zero extents for some paths with no inked area, (such as a simple line segment).

        Note that `fill_extents()` must necessarily do more work to compute the precise inked areas in light of the fill rule, so `Context.path_extents()` may be more desirable for sake of performance if the non-inked path extents are desired.

        See `Context.fill()`, `Context.set_fill_rule()` and `Context.fill_preserve()`.

        Returns
        -------
        >>> (x1: float, y1: float, x2: float, y2: float)\n
        - `x1 (float)` -- Left of the resulting extents
        - `y1 (float)` -- Top of the resulting extents
        - `x2 (float)` -- Right of the resulting extents
        - `y2 (float)` -- Bottom of the resulting extents
        """
    def fill_preserve(self) -> None:
        """
        A drawing operator that fills the current path according to the current `fill rule`, (each sub-path is implicitly closed before being filled). Unlike `Context.fill()`, `fill_preserve()` preserves the path within the `Context`.

        See `Context.set_fill_rule()` and `Context.fill()`.
        """
    def font_extents(self) -> Tuple[float, float, float, float, float]:
        """
        Gets the font extents for the currently selected font.

        Returns
        -------
        >>> (ascent: float, descent: float, height: float, max_x_advance: float, max_y_advance: float)\n
        """
    def get_antialias(self) -> Antialias:
        """
        Returns
        -------
        >>> cairo.Antialias\n
        The current antialias mode, as set by `Context.set_antialias()`.
        """
    def get_current_point(self) -> Tuple[float, float]:
        """
        Gets the current point of the current path, which is conceptually the final point reached by the path so far.

        The current point is returned in the user-space coordinate system. If there is no defined current point or if `Context` is in an error status, x and y will both be set to 0.0. It is possible to check this in advance with `Context.has_current_point()`.

        Most path construction functions alter the current point. See the following for details on how they affect the current point: `Context.new_path()`, `Context.new_sub_path()`, `Context.append_path()`, `Context.close_path()`, `Context.move_to()`, `Context.line_to()`, `Context.curve_to()`, `Context.rel_move_to()`, `Context.rel_line_to()`, `Context.rel_curve_to()`, `Context.arc()`, `Context.arc_negative()`, `Context.rectangle()`, `Context.text_path()`, `Context.glyph_path()`.

        Some functions use and alter the current point but do not otherwise change current path: `Context.show_text()`.

        Some functions unset the current path and as a result, current point: `Context.fill()`, `Context.stroke()`.

        Returns
        -------
        >>> (x: float, y: float)\n
        - `x (float)` -- X coordinate of the current point.
        - `y (float)` -- Y coordinate of the current point.
        """
    def get_dash(self) -> Tuple[List[float], float]:
        """
        Gets the current dash array.

        New in version 1.4.

        Returns
        -------
        >>> (dashes: list(float), offset: float)\n
        - `dashes list(float)` -- Return value as a tuple for the dash array
        - `offset (float)` -- Return value as a float for the current dash offset
        """
    def get_dash_count(self) -> int:
        """
        See also `Context.set_dash()` and `Context.get_dash()`.

        New in version 1.4.

        Returns
        -------
        >>> int\n
        The length of the dash array, or 0 if no dash array set.
        """
    def get_fill_rule(self) -> FillRule:
        """
        Returns
        -------
        >>> cairo.FillRule\n
        The current fill rule, as set by `Context.set_fill_rule()`.
        """
    def get_font_face(self) -> FontFace:
        """
        Returns
        -------
        >>> cairo.FontFace\n
        The current `FontFace` for the `Context`.
        """
    def get_font_matrix(self) -> Matrix:
        """
        See `Context.set_font_matrix()`.

        Returns
        -------
        >>> cairo.Matrix\n
        The current `Matrix` for the `Context`.
        """
    def get_font_options(self) -> FontOptions:
        """
        Retrieves font rendering options set via `Context.set_font_options()`. Note that the returned options do not include any options derived from the underlying surface; they are literally the options passed to `Context.set_font_options()`.

        Returns
        -------
        >>> cairo.FontOptions\n
        The current `FontOptions` for the `Context`.
        """
    def get_group_target(self) -> Surface:
        """
        Gets the current destination `Surface` for the `Context`. This is either the original target surface as passed to `Context` or the target surface for the current group as started by the most recent call to `Context.push_group()` or `Context.push_group_with_content()`.

        New in version 1.2.

        Returns
        -------
        >>> cairo.Surface\n
        The target `Surface`.
        """
    def get_line_cap(self) -> LineCap:
        """
        Returns
        -------
        >>> cairo.LineCap\n
        The current line cap style, as set by `Context.set_line_cap()`.
        """
    def get_line_join(self) -> LineJoin:
        """
        Returns
        -------
        >>> cairo.LineJoin\n
        The current line join style, as set by `Context.set_line_join()`.
        """
    def get_line_width(self) -> float:
        """
        This function returns the current line width value exactly as set by `Context.set_line_width()`. Note that the value is unchanged even if the CTM has changed between the calls to `Context.set_line_width()` and `get_line_width()`.

        Returns
        -------
        >>> float\n
        The current line width.
        """
    def get_matrix(self) -> Matrix:
        """
        Returns
        -------
        >>> cairo.Matrix\n
        The current transformation `Matrix` (CTM).
        """
    def get_miter_limit(self) -> float:
        """
        Returns
        -------
        >>> float\n
        The current miter limit, as set by `Context.set_miter_limit()`.
        """
    def get_operator(self) -> Operator:
        """
        Returns
        -------
        >>> cairo.Operator\n
        The current compositing operator for a `Context`.
        """
    def get_scaled_font(self) -> ScaledFont:
        """
        New in version 1.4.

        Returns
        -------
        >>> cairo.ScaledFont\n
        The current `ScaledFont`for a `Context`.
        """
    def get_source(self) -> Pattern:
        """
        Returns
        -------
        >>> cairo.Pattern\n
        The current source `Pattern` for a `Context`.
        """
    def get_target(self) -> Surface:
        """
        Returns
        -------
        >>> cairo.Surface\n
        The target `Surface` for the `Context`.
        """
    def get_tolerance(self) -> float:
        """
        >>> float\n
        The current tolerance value, as set by `Context.set_tolerance()`.
        """
    def glyph_extents(self, glyphs: Sequence[Glyph]) -> TextExtents:
        """
        Gets the extents for an array of glyphs. The extents describe a user-space rectangle that encloses the “inked” portion of the glyphs, (as they would be drawn by `Context.show_glyphs()`). Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by `Context.show_glyphs()`.

        Note that whitespace glyphs do not contribute to the size of the rectangle (extents.width and extents.height).

        Returns
        -------
        >>> cairo.TextExtents\n
        """
    def glyph_path(self, glyphs: Sequence[Glyph]) -> None:
        """
        Adds closed paths for the glyphs to the current path. The generated path if filled, achieves an effect similar to that of `Context.show_glyphs()`.

        Parameters
        ----------
        >>> glyphs (list(cairo.Glyph))\n
        Glyphs to show, a sequence of `Glyph`.
        """
    def has_current_point(self) -> bool:
        """
        See `Context.get_current_point()` for details on the current point.

        New in version 1.6.

        Returns
        -------
        >>> bool\n
        `True` if a current point is defined on the current path.
        """
    def identity_matrix(self) -> None:
        """
        Resets the current transformation `Matrix` (CTM) by setting it equal to the identity matrix. That is, the user-space and device-space axes will be aligned and one user-space unit will transform to one device-space unit.
        """
    def in_fill(self, x: float, y: float) -> bool:
        """
        See `Context.fill()`, `Context.set_fill_rule()` and `Context.fill_preserve()`.

        Parameters
        ----------
        >>> x (float)\n
        X coordinate of the point to test.
        >>> y (float)\n
        Y coordinate of the point to test.

        Returns
        -------
        >>> bool\n
        True if the point is inside the area that would be affected by a `Context.fill()` operation given the current path and filling parameters. Surface dimensions and clipping are not taken into account.
        """
    def in_stroke(self, x: float, y: float) -> bool:
        """
        See `Context.stroke()`, `Context.set_line_width()`, `Context.set_line_join()`, `Context.set_line_cap()`, `Context.set_dash()`, and `Context.stroke_preserve()`.

        Parameters
        ----------
        >>> x (float)\n
        X coordinate of the point to test.
        >>> y (float)\n
        Y coordinate of the point to test.

        Returns
        -------
        >>> bool\n
        True if the point is inside the area that would be affected by a `Context.stroke()` operation given the current path and filling parameters. Surface dimensions and clipping are not taken into account.
        """
    def line_to(self, x: float, y: float) -> None:
        """
        Adds a line to the path from the current point to position (x, y) in user-space coordinates. After this call the current point will be (x, y).

        If there is no current point before the call to `line_to()` this function will behave as `ctx.move_to(x, y)`.

        Parameters
        ----------
        >>> x (float)\n
        The X coordinate of the end of the new line.
        >>> y (float)\n
        The Y coordinate of the end of the new line.
        """
    def mask(self, pattern: Pattern) -> None:
        """
        A drawing operator that paints the current source using the alpha channel of pattern as a mask. (Opaque areas of pattern are painted with the source, transparent areas are not painted.)

        Parameters
        ----------
        >>> pattern (cairo.Pattern)\n
        """
    def mask_surface(self, surface: Surface, x: float = 0.0, y: float = 0.0) -> None:
        """
        A drawing operator that paints the current source using the alpha channel of surface as a mask. (Opaque areas of surface are painted with the source, transparent areas are not painted.)

        Parameters
        ----------
        >>> surface (cairo.Surface)\n
        A `Surface`.
        >>> x (float)\n
        X coordinate at which to place the origin of `surface`.
        >>> y (float)\n
        Y coordinate at which to place the origin of `surface`.
        """
    def move_to(self, x: float, y: float) -> None:
        """
        Begin a new sub-path. After this call the current point will be (x, y).

        Parameters
        ----------
        >>> x (float)\n
        The X coordinate of the new position.
        >>> y (float)\n
        The Y coordinate of the new position.
        """
    def new_path(self) -> None:
        """
        Clears the current path. After this call there will be no path and no current point.
        """
    def new_sub_path(self) -> None:
        """
        Begin a new sub-path. Note that the existing path is not affected. After this call there will be no current point.

        In many cases, this call is not needed since new sub-paths are frequently started with `Context.move_to()`.

        A call to `new_sub_path()` is particularly useful when beginning a new sub-path with one of the `Context.arc()` calls. This makes things easier as it is no longer necessary to manually compute the arc’s initial coordinates for a call to `Context.move_to()`.

        New in version 1.6.
        """
    def paint(self) -> None:
        """
        A drawing operator that paints the current source everywhere within the current clip region.
        """
    def paint_with_alpha(self, alpha: float) -> None:
        """
        A drawing operator that paints the current source everywhere within the current clip region using a mask of constant alpha value alpha. The effect is similar to `Context.paint()`, but the drawing is faded out using the alpha value.

        Parameters
        ----------
        >>> alpha (float)\n
        Alpha value, between 0 (transparent) and 1 (opaque).
        """
    def path_extents(self) -> Tuple[float, float, float, float]:
        """
        Computes a bounding box in user-space coordinates covering the points on the current path. If the current path is empty, returns an empty rectangle (0, 0, 0, 0). Stroke parameters, fill rule, surface dimensions and clipping are not taken into account.

        Contrast with `Context.fill_extents()` and `Context.stroke_extents()` which return the extents of only the area that would be “inked” by the corresponding drawing operations.

        The result of `path_extents()` is defined as equivalent to the limit of `Context.stroke_extents()` with `cairo.LINE_CAP_ROUND` as the line width approaches 0.0, (but never reaching the empty-rectangle returned by `Context.stroke_extents()` for a line width of 0.0).

        Specifically, this means that zero-area sub-paths such as `Context.move_to()`; `Context.line_to()` segments, (even degenerate cases where the coordinates to both calls are identical), will be considered as contributing to the extents. However, a lone `Context.move_to()` will not contribute to the results of `Context.path_extents()`.

        New in version 1.6.

        Returns
        -------
        >>> (x1: float, y1: float, x2: float, y2: float)\n
        - `x1 (float)` -- Left of the resulting extents.
        - `y1 (float)` -- Top of the resulting extents.
        - `x2 (float)` -- Right of the resulting extents.
        - `y2 (float)` -- Bottom of the resulting extents.
        """
    def pop_group(self) -> SurfacePattern:
        """
        Terminates the redirection begun by a call to `Context.push_group()` or `Context.push_group_with_content()` and returns a new pattern containing the results of all drawing operations performed to the group.

        The `pop_group()` function calls `Context.restore()`, (balancing a call to `Context.save()` by the ` Context.push_group()` function), so that any changes to the graphics state will not be visible outside the group.

        New in version 1.2.

        Returns
        -------
        >>> cairo.SurfacePattern\n
        A newly created `SurfacePattern` containing the results of all drawing operations performed to the group.
        """
    def pop_group_to_source(self) -> None:
        """
        Terminates the redirection begun by a call to `Context.push_group()` or `Context.push_group_with_content()` and installs the resulting pattern as the source `Pattern` in the given `Context`.

        The behavior of this function is equivalent to the sequence of operations:
        >>> group = cairo_pop_group()
        >>> ctx.set_source(group)\n

        but is more convenient as their is no need for a variable to store the short-lived pointer to the pattern.

        The `Context.pop_group()` function calls `Context.restore()`, (balancing a call to `Context.save()` by the `Context.push_group()` function), so that any changes to the graphics state will not be visible outside the group.

        New in version 1.2.
        """
    def push_group(self) -> None:
        """
        Temporarily redirects drawing to an intermediate surface known as a group. The redirection lasts until the group is completed by a call to `Context.pop_group()` or `Context.pop_group_to_source()`. These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).

        This group functionality can be convenient for performing intermediate compositing. One common use of a group is to render objects as opaque within the group, (so that they occlude each other), and then blend the result with translucence onto the destination.

        Groups can be nested arbitrarily deep by making balanced calls to `Context.push_group()`/`Context.pop_group()`. Each call pushes/pops the new target group onto/from a stack.

        The `push_group()` function calls `Context.save()` so that any changes to the graphics state will not be visible outside the group, (the `pop_group` functions call `Context.restore()`).

        By default the intermediate group will have a cairo.Content type of `cairo.Content.COLOR_ALPHA`. Other content types can be chosen for the group by using `Context.push_group_with_content()` instead.

        As an example, here is how one might fill and stroke a path with translucence, but without any portion of the fill being visible under the stroke:
        >>> ctx.push_group()
        >>> ctx.set_source(fill_pattern)
        >>> ctx.fill_preserve()
        >>> ctx.set_source(stroke_pattern)
        >>> ctx.stroke()
        >>> ctx.pop_group_to_source()
        >>> ctx.paint_with_alpha(alpha)

        New in version 1.2.
        """
    def push_group_with_content(self, content: Content) -> None:
        """
        Temporarily redirects drawing to an intermediate surface known as a group. The redirection lasts until the group is completed by a call to `Context.pop_group()` or `Context.pop_group_to_source()`. These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).

        The group will have a content type of content. The ability to control this content type is the only distinction between this function and `Context.push_group()` which you should see for a more detailed description of group rendering.

        New in version 1.2.

        Parameters
        ----------
        >>> content (cairo.Content)\n
        A content indicating the type of group that will be created.
        """
    def rectangle(self, x: float, y: float, width: float, height: float) -> None:
        """
        Adds a closed sub-path rectangle of the given size to the current path at position (x, y) in user-space coordinates.

        This function is logically equivalent to:
        >>> ctx.move_to(x, y)
        >>> ctx.rel_line_to(width, 0)
        >>> ctx.rel_line_to(0, height)
        >>> ctx.rel_line_to(-width, 0)
        >>> ctx.close_path()

        Parameters
        ----------
        >>> x (float)\n
        The X coordinate of the top left corner of the rectangle.
        >>> y (float)\n
        The Y coordinate of the top left corner of the rectangle.
        >>> width (float)\n
        The width of the rectangle.
        >>> height (float)\n
        The height of the rectangle.
        """
    def rel_curve_to(self, dx1: float, dy1: float, dx2: float, dy2: float, dx3: float, dy3: float) -> None:
        """
        Relative-coordinate version of `Context.curve_to()`. All offsets are relative to the current point. Adds a cubic Bézier spline to the path from the current point to a point offset from the current point by (dx3, dy3), using points offset by (dx1, dy1) and (dx2, dy2) as the control points. After this call the current point will be offset by (dx3, dy3).

        Given a current point of (x, y), `ctx.rel_curve_to(dx1, dy1, dx2, dy2, dx3, dy3)` is logically equivalent to `ctx.curve_to(x+dx1, y+dy1, x+dx2, y+dy2, x+dx3, y+dy3)`.

        Parameters
        ----------
        >>> dx1 (float)\n
        The X offset to the first control point.
        >>> dy1 (float)\n
        The Y offset to the first control point.
        >>> dx2 (float)\n
        The X offset to the second control point.
        >>> dy2 (float)\n
        The Y offset to the second control point.
        >>> dx3 (float)\n
        The X offset to the end of the curve.
        >>> dy3 (float)\n
        The Y offset to the end of the curve.

        Raises
        ------
        >>> cairo.Error\n
        If called with no current point.
        """
    def rel_line_to(self, dx: float, dy: float) -> None:
        """
        Relative-coordinate version of `Context.line_to()`. Adds a line to the path from the current point to a point that is offset from the current point by (dx, dy) in user space. After this call the current point will be offset by (dx, dy).

        Given a current point of (x, y), `ctx.rel_line_to(dx, dy)` is logically equivalent to `ctx.line_to(x + dx, y + dy)`.

        Parameters
        ----------
        >>> dx (float)\n
        The X offset to the end of the new line.
        >>> dy (float)\n
        The Y offset to the end of the new line.

        Raises
        ------
        >>> cairo.Error\n
        If called with no current point.
        """
    def rel_move_to(self, dx: float, dy: float) -> None:
        """
        Begin a new sub-path. After this call the current point will offset by (dx, dy).

        Given a current point of (x, y), `ctx.rel_move_to(dx, dy)` is logically equivalent to `ctx.(x + dx, y + dy)`.

        Parameters
        ----------
        >>> dx (float)\n
        The X offset.
        >>> dy (float)\n
        The Y offset.

        Raises
        ------
        >>> cairo.Error\n
        If called with no current point.
        """
    def reset_clip(self) -> None:
        """
        Reset the current clip region to its original, unrestricted state. That is, set the clip region to an infinitely large shape containing the target surface. Equivalently, if infinity is too hard to grasp, one can imagine the clip region being reset to the exact bounds of the target surface.

        Note that code meant to be reusable should not call `reset_clip()` as it will cause results unexpected by higher-level code which calls `clip()`. Consider using `save()` and `restore()` around `clip()` as a more robust means of temporarily restricting the clip region.
        """
    def restore(self) -> None:
        """
        Restores `Context` to the state saved by a preceding call to `save()` and removes that state from the stack of saved states.
        """
    def rotate(self, angle: float) -> None:
        """
        Modifies the current transformation matrix (CTM) by rotating the user-space axes by angle radians. The rotation of the axes takes places after any existing transformation of user space. The rotation direction for positive angles is from the positive X axis toward the positive Y axis.

        Parameters
        ----------
        >>> angle (float)\n
        Angle (in radians) by which the user-space axes will be rotated.
        """
    def save(self) -> None:
        """
        Makes a copy of the current state of Context and saves it on an internal stack of saved states. When `restore()` is called, `Context` will be restored to the saved state. Multiple calls to `save()` and `restore()` can be nested; each call to `restore()` restores the state from the matching paired `save()`.
        """
    def scale(self, sx: float, sy: float) -> None:
        """
        Modifies the current transformation matrix (CTM) by scaling the X and Y user-space axes by sx and sy respectively. The scaling of the axes takes place after any existing transformation of user space.

        Parameters
        ----------
        >>> sx (float)\n
        Scale factor for the X dimension.
        >>> sy (float)\n
        Scale factor for the Y dimension.
        """
    def select_font_face(self, family: str, slant: FontSlant = FontSlant.NORMAL, weight: FontWeight = FontWeight.NORMAL) -> None:
        """
        Note: The `select_font_face()` function call is part of what the cairo designers call the “toy” text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications.

        Selects a family and style of font from a simplified description as a family name, slant and weight. Cairo provides no operation to list available family names on the system (this is a “toy”, remember), but the standard CSS2 generic family names, (“serif”, “sans-serif”, “cursive”, “fantasy”, “monospace”), are likely to work as expected.

        For “real” font selection, see the font-backend-specific `font_face_create` functions for the font backend you are using. (For example, if you are using the freetype-based cairo-ft font backend, see `cairo_ft_font_face_create_for_ft_face()` or `cairo_ft_font_face_create_for_pattern()`.) The resulting font face could then be used with `cairo_scaled_font_create()` and `cairo_set_scaled_font()`.

        Similarly, when using the “real” font support, you can call directly into the underlying font system, (such as fontconfig or freetype), for operations such as listing available fonts, etc.

        It is expected that most applications will need to use a more comprehensive font handling and text layout library, (for example, pango), in conjunction with cairo.

        If text is drawn without a call to `select_font_face()`, (nor `set_font_face()` nor `set_scaled_font()`), the default family is platform-specific, but is essentially “sans-serif”. Default slant is `cairo.FontSlant.NORMAL`, and default weight is cairo.`FontWeight.NORMAL`.

        This function is equivalent to a call to `ToyFontFace` followed by `set_font_face()`.

        Parameters
        ----------
        >>> family (str)\n
        A font family name.
        >>> slant (cairo.FontSlant)\n
        The slant of the font, defaults to `cairo.FontSlant.NORMAL`.
        >>> weight (cairo.FontWeight)\n
        The weight of the font, defaults to `cairo.FontWeight.NORMAL`.
        """
    def set_antialias(self, antialias: Antialias) -> None:
        """
        Set the antialiasing mode of the rasterizer used for drawing shapes. This value is a hint, and a particular backend may or may not support a particular value. At the current time, no backend supports `cairo.Antialias.SUBPIXEL` when drawing shapes.

        Note that this option does not affect text rendering, instead see `FontOptions.set_antialias()`.

        Parameters
        ----------
        >>> antialias (cairo.Antialias)\n
        The new antialias mode.
        """
    def set_dash(self, dashes: Sequence[float], offset: float = 0) -> None:
        """
        Sets the dash pattern to be used by `stroke()`. A dash pattern is specified by dashes - a sequence of positive values. Each value provides the length of alternate “on” and “off” portions of the stroke. The offset specifies an offset into the pattern at which the stroke begins.

        Each “on” segment will have caps applied as if the segment were a separate sub-path. In particular, it is valid to use an “on” length of 0.0 with `cairo.LineCap.ROUND` or `cairo.LineCap.SQUARE` in order to distributed dots or squares along a path.

        Note: The length values are in user-space units as evaluated at the time of stroking. This is not necessarily the same as the user space at the time of `set_dash()`.

        If the number of dashes is 0 dashing is disabled.

        If the number of dashes is 1 a symmetric pattern is assumed with alternating on and off portions of the size specified by the single value in dashes.

        Parameters
        ----------
        >>> dashes (Sequence(float))\n
        A sequence specifying alternate lengths of on and off stroke portions as float.
        >>> offset (float)\n
        An offset into the dash pattern at which the stroke should start, defaults to 0.

        Raises
        ------
        >>> cairo.Error\n
        If any value in dashes is negative, or if all values are 0.
        """
    def set_fill_rule(self, fill_rule: FillRule) -> None:
        """
        The default fill rule is `cairo.FillRule.WINDING`.

        Parameters
        ----------
        >>> fill_rule (cairo.FillRule)\n
        A fill rule to set within the cairo context. The fill rule is used to determine which regions are inside or outside a complex (potentially self-intersecting) path. The current fill rule affects both `fill()` and `clip()`.
        """
    def set_font_face(self, font_face: Optional[FontFace]) -> None:
        """
        Replaces the current `FontFace` object in the `Context` with `ont_face`.

        Parameters
        ----------
        >>> font_face (cairo.FontFace or None)\n
        A `FontFace`, or `None` to restore to the default `FontFace`.
        """
    def set_font_matrix(self, matrix: Matrix) -> None:
        """
        Sets the current font matrix to matrix. The font matrix gives a transformation from the design space of the font (in this space, the em-square is 1 unit by 1 unit) to user space. Normally, a simple scale is used (see `set_font_size()`), but a more complex font matrix can be used to shear the font or stretch it unequally along the two axes.

        Parameters
        ----------
        >>> matrix (cairo.Matrix)\n
        A `Matrix` describing a transform to be applied to the current font.
        """
    def set_font_options(self, options: FontOptions) -> None:
        """
        Sets a set of custom font rendering options for the Context. Rendering options are derived by merging these options with the options derived from underlying surface; if the value in options has a default value (like `cairo.Antialias.DEFAULT`), then the value from the surface is used.

        Parameters
        ----------
        >>> options (cairo.FontOptions)\n
        `FontOptions` to use.
        """
    def set_font_size(self, size: float) -> None:
        """
        Sets the current font matrix to a scale by a factor of size, replacing any font matrix previously set with `set_font_size()` or `set_font_matrix()`. This results in a font size of size user space units. (More precisely, this matrix will result in the font’s em-square being a size by size square in user space.)

        If text is drawn without a call to `set_font_size()`, (nor `set_font_matrix()` nor `set_scaled_font()`), the default font size is 10.0.

        Parameters
        ----------
        >>> size (float)\n
        The new font size, in user space units.
        """
    def set_line_cap(self, line_cap: LineCap) -> None:
        """
        Sets the current line cap style within the Context.

        As with the other stroke parameters, the current line cap style is examined by `stroke()`, `stroke_extents()`, and `stroke_to_path()`, but does not have any effect during path construction.

        The default line cap style is `cairo.LineCap.BUTT`.

        Parameters
        ----------
        >>> line_cap (cairo.LineCap)\n
        A line cap style.
        """
    def set_line_join(self, line_join: LineJoin) -> None:
        """
        Sets the current line join style within the Context.

        As with the other stroke parameters, the current line join style is examined by `stroke()`, `stroke_extents()`, and `stroke_to_path()`, but does not have any effect during path construction.

        The default line join style is `cairo.LineJoin.MITER`.

        Parameters
        ----------
        >>> line_join (cairo.LineJoin)\n
        A line join style.
        """
    def set_line_width(self, width: float) -> None:
        """
        Sets the current line width within the `Context`. The line width value specifies the diameter of a pen that is circular in user space, (though device-space pen may be an ellipse in general due to scaling/shear/rotation of the CTM).

        Note: When the description above refers to user space and CTM it refers to the user space and CTM in effect at the time of the stroking operation, not the user space and CTM in effect at the time of the call to `set_line_width()`. The simplest usage makes both of these spaces identical. That is, if there is no change to the CTM between a call to `set_line_width()` and the stroking operation, then one can just pass user-space values to `set_line_width()` and ignore this note.

        As with the other stroke parameters, the current line width is examined by `stroke()`, `stroke_extents()`, and `stroke_to_path()`, but does not have any effect during path construction.

        The default line width value is 2.0.

        Parameters
        ----------
        >>> width (float)\n
        A line width.
        """
    def set_matrix(self, matrix: Matrix) -> None:
        """
        Modifies the current transformation matrix (CTM) by setting it equal to matrix.

        Parameters
        ----------
        >>> matrix (cairo.Matrix)\n
        A transformation `Matrix` from user space to device space.
        """
    def set_miter_limit(self, limit: float) -> None:
        """
        Sets the current miter limit within the Context.

        If the current line join style is set to cairo.LineJoin.MITER (see `set_line_join()`), the miter limit is used to determine whether the lines should be joined with a bevel instead of a miter. Cairo divides the length of the miter by the line width. If the result is greater than the miter limit, the style is converted to a bevel.

        As with the other stroke parameters, the current line miter limit is examined by `stroke()`, `stroke_extents()`, and `stroke_to_path()`, but does not have any effect during path construction.

        The default miter limit value is 10.0, which will convert joins with interior angles less than 11 degrees to bevels instead of miters. For reference, a miter limit of 2.0 makes the miter cutoff at 60 degrees, and a miter limit of 1.414 makes the cutoff at 90 degrees.

        A miter limit for a desired angle can be computed as:
        >>> miter limit = 1/math.sin(angle/2)

        Parameters
        ----------
        >>> limit (float)\n
        Miter limit to set.
        """
    def set_operator(self, op: Operator) -> None:
        """
        The default operator is `cairo.Operator.OVER`.

        Parameters
        ----------
        >>> op (cairo.Operator)\n
        The compositing operator to set for use in all drawing operations.
        """
    def set_scaled_font(self, scaled_font: ScaledFont) -> None:
        """
        Replaces the current font face, font matrix, and font options in the `Context` with those of the `ScaledFont`. Except for some translation, the current CTM of the `Context` should be the same as that of the `ScaledFont`, which can be accessed using `ScaledFont.get_ctm()`.

        New in version 1.2.

        Parameters
        ----------
        >>> scaled_font (cairo.ScaledFont)\n
        """
    def set_source(self, source: Pattern) -> None:
        """
        Sets the source pattern within `Context` to source. This pattern will then be used for any subsequent drawing operation until a new source pattern is set.

        Note: The pattern’s transformation matrix will be locked to the user space in effect at the time of `set_source()`. This means that further modifications of the current transformation matrix will not affect the source pattern. See `Pattern.set_matrix()`.

        The default source pattern is a solid pattern that is opaque black, (that is, it is equivalent to `set_source_rgb(0.0, 0.0, 0.0)`.

        Parameters
        ----------
        >>> source (cairo.Pattern)\n
        A `Pattern` to be used as the source for subsequent drawing operations.
        """
    def set_source_rgb(self, red: float, green: float, blue: float) -> None:
        """
        Sets the source pattern within `Context` to an opaque color. This opaque color will then be used for any subsequent drawing operation until a new source pattern is set.

        The color components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.

        The default source pattern is opaque black, (that is, it is equivalent to `set_source_rgb(0.0, 0.0, 0.0)`.

        Parameters
        ----------
        >>> red (float)\n
        Red component of color.
        >>> green (float)\n
        Green component of color.
        >>> blue (float)\n
        Blue component of color.
        """
    def set_source_rgba(self, red: float, green: float, blue: float, alpha: float = 1.0) -> None:
        """
        Sets the source pattern within `Context` to a translucent color. This color will then be used for any subsequent drawing operation until a new source pattern is set.

        The color and alpha components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.

        The default source pattern is opaque black, (that is, it is equivalent to `set_source_rgba(0.0, 0.0, 0.0, 1.0)`.

        Parameters
        ----------
        >>> red (float)\n
        Red component of color.
        >>> green (float)\n
        Green component of color.
        >>> blue (float)\n
        Blue component of color.
        >>> alpha (float)\n
        Alpha component of color.
        """
    def set_source_surface(self, surface: Surface, x: float = 0.0, y: float = 0.0) -> None:
        """
        This is a convenience function for creating a pattern from a Surface and setting it as the source in `Context` with `set_source()`.

        The x and y parameters give the user-space coordinate at which the surface origin should appear. (The surface origin is its upper-left corner before any transformation has been applied.) The x and y patterns are negated and then set as translation values in the pattern matrix.

        Other than the initial translation pattern matrix, as described above, all other pattern attributes, (such as its extend mode), are set to the default values as in `SurfacePattern`. The resulting pattern can be queried with `get_source()` so that these attributes can be modified if desired, (eg. to create a repeating pattern with `Pattern.set_extend()`).

        Parameters
        ----------
        >>> surface (cairo.Surface)\n
        A `Surface` to be used to set the source pattern.
        >>> x (float)\n
        User-space X coordinate for the surface's origin.
        >>> y (float)\n
        User-space Y coordinate for the surface's origin.
        """
    def set_tolerance(self, tolerance: float) -> None:
        """
        Sets the tolerance used when converting paths into trapezoids. Curved segments of the path will be subdivided until the maximum deviation between the original path and the polygonal approximation is less than tolerance. The default value is 0.1. A larger value will give better performance, a smaller value, better appearance. (Reducing the value from the default value of 0.1 is unlikely to improve appearance significantly.) The accuracy of paths within Cairo is limited by the precision of its internal arithmetic, and the prescribed tolerance is restricted to the smallest representable internal value.

        Parameters
        ----------
        >>> tolerance (float)\n
        The tolerance, in device units (typically pixels).
        """
    def show_glyphs(self, glyphs: Sequence[Glyph]) -> None:
        """
        A drawing operator that generates the shape from an array of glyphs, rendered according to the current font face, font size (font matrix), and font options.

        Parameters
        ----------
        >>> glyphs (Sequence(cairo.Glyph))\n
        Glyphs to show as a sequence of `Glyph`.
        """
    def show_page(self) -> None:
        """
        Emits and clears the current page for backends that support multiple pages. Use `copy_page()` if you don’t want to clear the page.

        This is a convenience function that simply calls `ctx.get_target().show_page()`
        """
    def show_text(self, text: str) -> None:
        """
        A drawing operator that generates the shape from a string of text, rendered according to the current font_face, font_size (font_matrix), and font_options.

        This function first computes a set of glyphs for the string of text. The first glyph is placed so that its origin is at the current point. The origin of each subsequent glyph is offset from that of the previous glyph by the advance values of the previous glyph.

        After this call the current point is moved to the origin of where the next glyph would be placed in this same progression. That is, the current point will be at the origin of the final glyph offset by its advance values. This allows for easy display of a single logical string with multiple calls to `show_text()`.

        Note: The `show_text()` function call is part of what the cairo designers call the “toy” text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications. See `show_glyphs()` for the “real” text display API in cairo.

        Parameters
        ----------
        >>> text (str)\n
        """
    def stroke(self) -> None:
        """
        A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. After `stroke()`, the current path will be cleared from the cairo context. See `set_line_width()`, `set_line_join()`, `set_line_cap()`, `set_dash()`, and `stroke_preserve()`.

        Note: Degenerate segments and sub-paths are treated specially and provide a useful result. These can result in two different situations:

        1. Zero-length “on” segments set in `set_dash()`. If the cap style is `cairo.LineCap.ROUND` or `cairo.LineCap.SQUARE` then these segments will be drawn as circular dots or squares respectively. In the case of `cairo.LineCap.SQUARE`, the orientation of the squares is determined by the direction of the underlying path.

        2. A sub-path created by `move_to()` followed by either a `close_path()` or one or more calls to `line_to()` to the same coordinate as the `move_to()`. If the cap style is `cairo.LineCap.ROUND` then these sub-paths will be drawn as circular dots. Note that in the case of `cairo.LineCap.SQUARE` a degenerate sub-path will not be drawn at all, (since the correct orientation is indeterminate).

        In no case will a cap style of `cairo.LineCap.BUTT` cause anything to be drawn in the case of either degenerate segments or sub-paths.
        """
    def stroke_extents(self) -> Tuple[float, float, float, float]:
        """
        Computes a bounding box in user coordinates covering the area that would be affected, (the “inked” area), by a `stroke()` operation given the current path and stroke parameters. If the current path is empty, returns an empty rectangle (0, 0, 0, 0). Surface dimensions and clipping are not taken into account.

        Note that if the line width is set to exactly zero, then `stroke_extents()` will return an empty rectangle. Contrast with `path_extents()` which can be used to compute the non-empty bounds as the line width approaches zero.

        Note that `stroke_extents()` must necessarily do more work to compute the precise inked areas in light of the stroke parameters, so `path_extents()` may be more desirable for sake of performance if non-inked path extents are desired.

        See `stroke()`, `set_line_width()`, `set_line_join()`, `set_line_cap()`, `set_dash()`, and `stroke_preserve()`.

        Returns
        ----------
        >>> (x1: float, y1: float, x2: float, y2: float)\n
        - `x1 (float)` -- Left of the resulting extents.
        - `y1 (float)` -- Top of the resulting extents.
        - `x2 (float)` -- Right of the resulting extents.
        - `y2 (float)` -- Bottom of the resulting extents.
        """
    def stroke_preserve(self) -> None:
        """
        A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. Unlike `stroke()`, `stroke_preserve()` preserves the path within the cairo context.

        See `set_line_width()`, `set_line_join()`, `set_line_cap()`, `set_dash()`, and `stroke_preserve()`.
        """
    def text_extents(self, text: str) -> TextExtents:
        """
        Gets the extents for a string of text. The extents describe a user-space rectangle that encloses the “inked” portion of the text, (as it would be drawn by `Context.show_text()`). Additionally, the x_advance and y_advance values indicate the amount by which the current point would be advanced by `Context.show_text()`.

        Note that whitespace characters do not directly contribute to the size of the rectangle (`extents.width` and `extents.height`). They do contribute indirectly by changing the position of non-whitespace characters. In particular, trailing whitespace characters are likely to not affect the size of the rectangle, though they will affect the `x_advance` and `y_advance` values.

        Parameters
        ----------
        >>> text (str)\n
        Text to get extents for.

        Returns
        -------
        >>> cairo.TextExtents\n
        """
    def text_path(self, text: str) -> None:
        """
        Adds closed paths for text to the current path. The generated path if filled, achieves an effect similar to that of `Context.show_text()`.

        Text conversion and positioning is done similar to `Context.show_text()`.

        Like `Context.show_text()`, After this call the current point is moved to the origin of where the next glyph would be placed in this same progression. That is, the current point will be at the origin of the final glyph offset by its advance values. This allows for chaining multiple calls to to `Context.text_path()` without having to set current point in between.

        Note: The `text_path()` function call is part of what the cairo designers call the “toy” text API. It is convenient for short demos and simple programs, but it is not expected to be adequate for serious text-using applications. See `Context.glyph_path()` for the “real” text path API in cairo.

        Parameters
        ----------
        >>> text (str)\n
        """
    def transform(self, matrix: Matrix) -> None:
        """
        Modifies the current transformation matrix (CTM) by applying matrix as an additional transformation. The new transformation of user space takes place after any existing transformation.

        Parameters
        ----------
        >>> matrix (cairo.Matrix)\n
        A transformation `Matrix` to be applied to the user-space axes.
        """
    def translate(self, tx: float, ty: float) -> None:
        """
        Modifies the current transformation matrix (CTM) by translating the user-space origin by (tx, ty). This offset is interpreted as a user-space coordinate according to the CTM in place before the new call to `translate()`. In other words, the translation of the user-space origin takes place after any existing transformation.

        Parameters
        ----------
        >>> tx (float)\n
        Amount to translate in the X direction.
        >>> ty (float)\n
        Amount to translate in the Y direction.
        """
    def user_to_device(self, x: float, y: float) -> Tuple[float, float]:
        """
        Transform a coordinate from user-space to device-space by multiplying the given point by the current transformation matrix (CTM).

        Parameters
        ----------
        >>> x (float)\n
        X value of coordinate.
        >>> y (float)\n
        Y value of coordinate.

        Returns
        -------
        >>> (x: float, y: float)\n
        - `x (float)` -- Converted device-space X coordinate.
        - `y (float)` -- Converted device-space Y coordinate.
        """
    def user_to_device_distance(self, dx: float, dy: float) -> Tuple[float, float]:
        """
        Transform a distance vector from user space to device space. This function is similar to `Context.user_to_device()` except that the translation components of the CTM will be ignored when transforming (dx,dy).

        Parameters
        ----------
        >>> dx (float)\n
        X value of a distance vector.
        >>> dy (float)\n
        Y value of a distance vector.

        Returns
        -------
        >>> (dx: float, dy: float)\n
        - `dx (float)` -- Converted to device-space X value of a distance vector.
        - `dy (float)` -- Converted to device-space Y value of a distance vector.
        """
    def in_clip(self, x: float, y: float) -> bool:
        """
        Tests whether the given point is inside the area that would be visible through the current clip, i.e. the area that would be filled by a `paint()` operation.

        See `clip()`, and `clip_preserve()`.

        New in version 1.12.0.

        Parameters
        ----------
        >>> x (float)\n
        X coordinate of the point to test.
        >>> y (float)\n
        Y coordinate of the point to test.

        Returns
        -------
        >>> bool\n
        `True` if the point is inside, or `False` if outside.
        """
    def show_text_glyphs(self, utf8: str, glyphs: List["Glyph"], clusters: List[TextCluster], cluster_flags: TextClusterFlags) -> None:
        """
        New in version 1.15.

        This operation has rendering effects similar to `Context.show_glyphs()` but, if the target surface supports it, uses the provided text and cluster mapping to embed the text for the glyphs shown in the output. If the target does not support the extended attributes, this function acts like the basic `Context.show_glyphs()` as if it had been passed `glyphs`.

        The mapping between `utf8` and `glyphs` is provided by a list of `clusters`. Each cluster covers a number of text bytes and glyphs, and neighboring clusters cover neighboring areas of `utf8` and glyphs . The clusters should collectively cover utf8 and glyphs in entirety.

        The first cluster always covers bytes from the beginning of utf8 . If `cluster_flags` do not have the `TextClusterFlags.BACKWARD` set, the first cluster also covers the beginning of glyphs , otherwise it covers the end of the glyphs array and following clusters move backward.

        See `TextCluster` for constraints on valid clusters.

        Parameters
        ----------
        >>> utf8 (str)\n
        A string of text.
        >>> glyphs (list(cairo.Glyph))\n
        A list of glyphs to show.
        >>> clusters (list(cairo.TextCluster))\n
        A list of cluster mapping information.
        >>> cluster_flags (cairo.TextClusterFlags)\n
        Cluster mapping flags.
        """
    def tag_begin(self, tag_name: str, attributes: str) -> None:
        """
        Marks the beginning of the tag_name structure. Call `tag_end()` with the same tag_name to mark the end of the structure.

        The attributes string is of the form “key1=value2 key2=value2 …”. Values may be boolean (true/false or 1/0), integer, float, string, or an array.

        String values are enclosed in single quotes (‘). Single quotes and backslashes inside the string should be escaped with a backslash.

        Boolean values may be set to true by only specifying the key. eg the attribute string “key” is the equivalent to “key=true”.

        Arrays are enclosed in ‘[]’. eg “rect=[1.2 4.3 2.0 3.0]”.

        If no attributes are required, attributes can be an empty string.

        See `Tags and Links Description` for the list of tags and attributes.

        Invalid nesting of tags or invalid attributes will cause the context to shutdown with a status of `Status.TAG_ERROR`.

        See `tag_end()`.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> tag_name (str)\n
        Tag name.
        >>> attributes (str)\n
        Tag attributes.
        """
    def tag_end(self, tag_name: str) -> None:
        """
        Marks the end of the `tag_name` structure.

        Invalid nesting of tags will cause the context to shutdown with a status of `Status.TAG_ERROR`.

        See `tag_begin()`.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> tag_name (str)\n
        The tag name.
        """

class Error(Exception):
    """This exception is raised when a cairo object returns an error status."""
    status: Status = ...

CairoError = Error

class Gradient(Pattern):
    """
    Gradient is an abstract base class from which other Pattern classes derive. It cannot be instantiated directly.
    """
    def add_color_stop_rgb(self, offset: float, red: float, green: float, blue: float) -> None:
        """
        Adds an opaque color stop to a Gradient pattern. The offset specifies the location along the gradient’s control vector. For example, a LinearGradient’s control vector is from (x0,y0) to (x1,y1) while a RadialGradient’s control vector is from any point on the start circle to the corresponding point on the end circle.

        The color is specified in the same way as in `Context.set_source_rgb()`.

        If two (or more) stops are specified with identical offset values, they will be sorted according to the order in which the stops are added, (stops added earlier will compare less than stops added later). This can be useful for reliably making sharp color transitions instead of the typical blend.

        Parameters
        ----------
        >>> offset (float)\n
        An offset in the range [0.0 .. 1.0].
        >>> red (float)\n
        Red component of a color.
        >>> green (float)\n
        Green component of a color.
        >>> blue (float)\n
        Blue component of a color.
        """
    def add_color_stop_rgba(self, offset: float, red: float, green: float, blue: float, alpha: float) -> None:
        """
        Adds an opaque color stop to a Gradient pattern. The offset specifies the location along the gradient’s control vector. For example, a LinearGradient’s control vector is from (x0,y0) to (x1,y1) while a RadialGradient’s control vector is from any point on the start circle to the corresponding point on the end circle.

        The color is specified in the same way as in `Context.set_source_rgb()`.

        If two (or more) stops are specified with identical offset values, they will be sorted according to the order in which the stops are added, (stops added earlier will compare less than stops added later). This can be useful for reliably making sharp color transitions instead of the typical blend.
        
        Parameters
        ----------
        >>> offset (float)\n
        An offset in the range [0.0 .. 1.0].
        >>> red (float)\n
        Red component of a color.
        >>> green (float)\n
        Green component of a color.
        >>> blue (float)\n
        Blue component of a color.
        >>> alpha (float)\n
        Alpha component of a color.
        """
    def get_color_stops_rgba(self) -> List[Tuple[float, float, float, float, float]]:
        """
        Gets the color and offset information for all color stops specified in the given gradient pattern.

        New in version 1.14.

        Returns
        -------
        >>> [(offset: float, red: float, green: float, blue: float, alpha: float),...]
        """

class LinearGradient(Gradient):
    """
    Create a new `LinearGradient` along the line defined by (x0, y0) and (x1, y1). Before using the `Gradient` pattern, a number of color stops should be defined using `Gradient.add_color_stop_rgb()` or `Gradient.add_color_stop_rgba()`

    Note: The coordinates here are in pattern space. For a new `Pattern`, pattern space is identical to user space, but the relationship between the spaces can be changed with `Pattern.set_matrix()`

    Parameters
    ----------
    >>> x0 (float)\n
    X coordinate of the start point.
    >>> y0 (float)\n
    Y coordinate of the start point.
    >>> x1 (float)\n
    X coordinate of the end point.
    >>> y1 (float)\n
    Y coordinate of the end point.

    Returns
    -------
    >>> LinearGradient\n
    A new `LinearGradient`.

    Raises
    ------
    >>> MemoryError\n
    In case of no memory.
    """
    def __init__(self, x0: float, y0: float, x1: float, y1: float) -> None: ...
    def get_linear_points(self) -> Tuple[float, float, float, float]:
        """
        Gets the gradient endpoints for a `LinearGradient`.

        New in version 1.4.

        Returns
        -------
        >>> (x0: float, y0: float, x1: float, y1: float)\n
        - `x0 (float)` - Return value for the x coordinate of the first point.
        - `y0 (float)` - Return value for the y coordinate of the first point.
        - `x1 (float)` - Return value for the x coordinate of the second point.
        - `y1 (float)` - Return value for the y coordinate of the second point.
        """

class MeshPattern(Pattern):
    """
    New in version 1.14.

    Create a new mesh pattern.

    Mesh patterns are tensor-product patch meshes (type 7 shadings in PDF). Mesh patterns may also be used to create other types of shadings that are special cases of tensor-product patch meshes such as Coons patch meshes (type 6 shading in PDF) and Gouraud-shaded triangle meshes (type 4 and 5 shadings in PDF).

    Mesh patterns consist of one or more tensor-product patches, which should be defined before using the mesh pattern. Using a mesh pattern with a partially defined patch as source or mask will put the context in an error status with a status of `cairo.Status.INVALID_MESH_CONSTRUCTION`.

    A tensor-product patch is defined by 4 Bézier curves (side 0, 1, 2, 3) and by 4 additional control points (P0, P1, P2, P3) that provide further control over the patch and complete the definition of the tensor-product patch. The corner C0 is the first point of the patch.

    Degenerate sides are permitted so straight lines may be used. A zero length line on one side may be used to create 3 sided patches.

    >>>      C1     Side 1      C2
    >>>        +---------------+
    >>>        |               |
    >>>        |  P1       P2  |
    >>>        |               |
    >>> Side 0 |               | Side 2
    >>>        |               |
    >>>        |               |
    >>>        |  P0       P3  |
    >>>        |               |
    >>>        +---------------+
    >>>      C0     Side 3      C3

    Each patch is constructed by first calling `begin_patch()`, then `move_to()` to specify the first point in the patch (C0). Then the sides are specified with calls to `curve_to()` and `line_to()`.

    The four additional control points (P0, P1, P2, P3) in a patch can be specified with `set_control_point()`.

    At each corner of the patch (C0, C1, C2, C3) a color may be specified with `set_corner_color_rgb()` or `set_corner_color_rgba()`. Any corner whose color is not explicitly specified defaults to transparent black.

    A Coons patch is a special case of the tensor-product patch where the control points are implicitly defined by the sides of the patch. The default value for any control point not specified is the implicit value for a Coons patch, i.e. if no control points are specified the patch is a Coons patch.

    A triangle is a special case of the tensor-product patch where the control points are implicitly defined by the sides of the patch, all the sides are lines and one of them has length 0, i.e. if the patch is specified using just 3 lines, it is a triangle. If the corners connected by the 0-length side have the same color, the patch is a Gouraud-shaded triangle.

    Patches may be oriented differently to the above diagram. For example the first point could be at the top left. The diagram only shows the relationship between the sides, corners and control points. Regardless of where the first point is located, when specifying colors, corner 0 will always be the first point, corner 1 the point between side 0 and side 1 etc.

    Calling `end_patch()` completes the current patch. If less than 4 sides have been defined, the first missing side is defined as a line from the current point to the first point of the patch (C0) and the other sides are degenerate lines from C0 to C0. The corners between the added sides will all be coincident with C0 of the patch and their color will be set to be the same as the color of C0.

    Additional patches may be added with additional calls to `begin_patch()`/`end_patch()`.

    >>> # Add a Coons patch
    >>> pattern = cairo.MeshPattern()
    >>> pattern.begin_patch()
    >>> pattern.move_to(0, 0)
    >>> pattern.curve_to(30, -30, 60, 30, 100, 0)
    >>> pattern.curve_to(60, 30, 130, 60, 100, 100)
    >>> pattern.curve_to(60, 70, 30, 130, 0, 100)
    >>> pattern.curve_to(30, 70, -30, 30, 0, 0)
    >>> pattern.set_corner_color_rgb(0, 1, 0, 0)
    >>> pattern.set_corner_color_rgb(1, 0, 1, 0)
    >>> pattern.set_corner_color_rgb(2, 0, 0, 1)
    >>> pattern.set_corner_color_rgb(3, 1, 1, 0)
    >>> pattern.end_patch()
    >>> 
    >>> # Add a Gouraud-shaded triangle
    >>> pattern = cairo.MeshPattern()
    >>> pattern.begin_patch()
    >>> pattern.move_to(100, 100)
    >>> pattern.line_to(130, 130)
    >>> pattern.line_to(130, 70)
    >>> pattern.set_corner_color_rgb(0, 1, 0, 0)
    >>> pattern.set_corner_color_rgb(1, 0, 1, 0)
    >>> pattern.set_corner_color_rgb(2, 0, 0, 1)
    >>> pattern.end_patch()

    When two patches overlap, the last one that has been added is drawn over the first one.

    When a patch folds over itself, points are sorted depending on their parameter coordinates inside the patch. The v coordinate ranges from 0 to 1 when moving from side 3 to side 1; the u coordinate ranges from 0 to 1 when going from side 0 to side

    Points with higher v coordinate hide points with lower v coordinate. When two points have the same v coordinate, the one with higher u coordinate is above. This means that points nearer to side 1 are above points nearer to side 3; when this is not sufficient to decide which point is above (for example when both points belong to side 1 or side 3) points nearer to side 2 are above points nearer to side 0.

    For a complete definition of tensor-product patches, see the PDF specification (ISO32000), which describes the parametrization in detail.

    Note: The coordinates are always in pattern space. For a new pattern, pattern space is identical to user space, but the relationship between the spaces can be changed with `Pattern.set_matrix()`.

    Returns
    -------
    >>> cairo.MeshPattern

    Raises
    ------
    >>> cairo.Error
    """
    def __init__(self) -> None: ...
    def begin_patch(self) -> None:
        """
        Begin a patch in a mesh pattern.

        After calling this function, the patch shape should be defined with `move_to()`, `line_to()` and `curve_to()`.

        After defining the patch, `end_patch()` must be called before using pattern as a source or mask.

        Raises
        ------
        >>> cairo.Error
        """
    def curve_to(self, x1: float, y1: float, x2: float, y2: float, x3: float, y3: float) -> None:
        """
        Adds a cubic Bézier spline to the current patch from the current point to position (x3 , y3 ) in pattern-space coordinates, using (x1 , y1 ) and (x2 , y2 ) as the control points.

        If the current patch has no current point before the call to `curve_to()`, this function will behave as if preceded by a call to `pattern.move_to(x1, y1)`.

        After this call the current point will be (x3 , y3 ).

        Parameters
        ----------
        >>> x1 (float)\n
        The X coordinate of the first control point.
        >>> y1 (float)\n
        The Y coordinate of the first control point.
        >>> x2 (float)\n
        The X coordinate of the second control point.
        >>> y2 (float)\n
        The Y coordinate of the second control point.
        >>> x3 (float)\n
        The X coordinate of the end control point.
        >>> y3 (float)\n
        The Y coordinate of the end control point.

        Raises
        ------
        >>> cairo.Error
        """
    def end_patch(self) -> None:
        """
        Indicates the end of the current patch in a mesh pattern.

        If the current patch has less than 4 sides, it is closed with a straight line from the current point to the first point of the patch as if `line_to()` was used.

        Raises
        ------
        >>> cairo.Error
        """
    def get_control_point(self, patch_num: int, point_num: int) -> Tuple[float, float]:
        """
        Gets the control point point_num of patch patch_num for a mesh pattern.

        `patch_num` can range from 0 to n-1 where n is the number returned by `get_patch_count()`.

        Valid values for `point_num` are from 0 to 3 and identify the control points as explained in `MeshPattern`.

        Parameters
        ----------
        >>> patch_num (int)\n
        The patch number to return data for.
        >>> point_num (int)\n
        The control point number to return data for.

        Returns
        -------
        >>> (x: float, y: float)\n
        Coordinates of the control point.

        Raises
        ------
        >>> cairo.Error
        """
    def get_corner_color_rgba(self, patch_num: int, corner_num: int) -> Tuple[float, float, float, float]:
        """
        Gets the color information in corner `corner_num` of patch `patch_num` for a mesh pattern.

        `patch_num` can range from 0 to n-1 where n is the number returned by `get_patch_count()`.

        Valid values for `corner_num` are from 0 to 3 and identify the corners as explained in `MeshPattern`.

        Parameters
        ----------
        >>> patch_num (int)\n
        The patch number to return data for.
        >>> corner_num (int)\n
        The corner number to return data for.

        Returns
        -------
        >>> (red: float, green: float, blue: float, alpha: float)\n
        RGBA components of the corner color.

        Raises
        ------
        >>> cairo.Error
        """
    def get_patch_count(self) -> int:
        """
        Gets the number of patches specified in the given mesh pattern.

        The number only includes patches which have been finished by calling `end_patch()`. For example it will be 0 during the definition of the first patch.

        Returns
        -------
        >>> int\n
        Number of patches.
        """
    def line_to(self, x: float, y: float) -> None:
        """
        Adds a line to the current patch from the current point to position (x, y) in pattern-space coordinates.

        If there is no current point before the call to `line_to()` this function will behave as `pattern.move_to(x, y)`.

        After this call the current point will be (x, y).

        Parameters
        ----------
        >>> x (float)\n
        The X coordinate of the end of the new line.
        >>> y (float)\n
        The Y coordinate of the end of the new line.

        Raises
        ------
        >>> cairo.Error
        """
    def move_to(self, x: float, y: float) -> None:
        """
        Define the first point of the current patch in a mesh pattern.

        After this call the current point will be (x, y).

        Parameters
        ----------
        >>> x (float)\n
        The X coordinate of the new position.
        >>> y (float)\n
        The Y coordinate of the new position.

        Raises
        ------
        >>> cairo.Error
        """
    def set_control_point(self, point_num: int, x: float, y: float) -> None:
        """
        Set an internal control point of the current patch.

        Valid values for `point_num` are from 0 to 3 and identify the control points as explained in `MeshPattern`.

        Parameters
        ----------
        >>> point_num (int)\n
        The control point to set the position for.
        >>> x (float)\n
        The X coordinate of the control point.
        >>> y (float)\n
        The Y coordinate of the control point.

        Raises
        ------
        >>> cairo.Error
        """
    def set_corner_color_rgb(self, corner_num: int, red: float, green: float, blue: float) -> None:
        """
        Sets the color of a corner of the current patch in a mesh pattern.

        The color is specified in the same way as in `Context.set_source_rgb()`.

        Valid values for `corner_num` are from 0 to 3 and identify the corners as explained in `MeshPattern`.

        Parameters
        ----------
        >>> corner_num (int)\n
        The corner to set the color for.
        >>> red (float)\n
        Red component of the color.
        >>> green (float)\n
        Green component of the color.
        >>> blue (float)\n
        Bleu component of the color.

        Raises
        ------
        >>> cairo.Error
        """
    def set_corner_color_rgba(self, corner_num: int, red: float, green: float, blue: float, alpha: float) -> None:
        """
        Sets the color of a corner of the current patch in a mesh pattern.

        The color is specified in the same way as in `Context.set_source_rgba()`.

        Valid values for `corner_num` are from 0 to 3 and identify the corners as explained in `MeshPattern`.

        Parameters
        ----------
        >>> corner_num (int)\n
        The corner to set the color for.
        >>> red (float)\n
        Red component of the color.
        >>> green (float)\n
        Green component of the color.
        >>> blue (float)\n
        Blue component of the color.
        >>> alpha (float)\n
        Alpha component of the color.

        Raises
        ------
        >>> cairo.Error
        """
    def get_path(self, patch_num: int) -> Path: ...

class PDFSurface(Surface):
    """
    The PDFSurface is used to render cairo graphics to Adobe PDF files and is a multi-page vector surface backend.

    New in version 1.2.

    Parameters
    ----------
    >>> fobj (str or None)\n
    (None, `pathlike`, file or file-like object) - A filename or writable file object. `None` may be used to specify no output. This will generate a PDFSurface that may be queried and used as a source, without generating a temporary file.
    >>> width_in_points (float)\n
    Width of the surface, in points (1 point == 1/72.0 inch).
    >>> height_in_points (float)\n
    Height of the surface, in points (1 point == 1/72.0 inch).

    Returns
    -------
    >>> cairo.PDFSurface\n
    A new PDFSurface of the specified size in points to be written to fobj.

    Raises
    ------
    >>> cairo.MemoryError\n
    In case of no memory.
    """
    def __init__(self, fobj: Union[_PathLike, _FileLike], width_in_points: float, height_in_points: float) -> None: ...
    def set_size(self, width_in_points: float, height_in_points: float) -> None:
        """
        Changes the size of a PDFSurface for the current (and subsequent) pages.

        This function should only be called before any drawing operations have been performed on the current page. The simplest way to do this is to call this function immediately after creating the surface or immediately after completing a page with either `Context.show_page()` or `Context.copy_page()`.

        New in version 1.2.

        Parameters
        ----------
        >>> width_in_ponts (float)\n
        New surface width, in points (1 point == 1/72.0 inch).
        >>> height_in_points (float)\n
        New surface height, in points (1 point == 1/72.0 inch).
        """
    def restrict_to_version(self, version: PDFVersion) -> None:
        """
        Restricts the generated PDF file to version . See `get_versions()` for a list of available version values that can be used here.

        This function should only be called before any drawing operations have been performed on the given surface. The simplest way to do this is to call this function immediately after creating the surface.

        New in version 1.12.0.

        Parameters
        ----------
        >>> version (cairo.PDFVersion)\n
        """
    @staticmethod
    def get_versions() -> List[PDFVersion]:
        """
        Retrieve the list of supported versions. See `restrict_to_version()`.

        New in version 1.12.0.

        Returns
        -------
        >>> list(cairo.PDFVersion)\n
        Supported version list.
        """
    @staticmethod
    def version_to_string(version: PDFVersion) -> str:
        """
        Get the string representation of the given version id. See `get_versions()` for a way to get the list of valid version ids.

        New in version 1.12.0.

        Parameters
        ----------
        >>> version (cairo.PDFVersion)\n

        Returns
        -------
        >>> str\n
        The string associated to the given version.

        Raises
        ------
        >>> ValueError\n
        If the version isn't valid.
        """
    def add_outline(self, parent_id: int, utf8: str, link_attribs: str, flags: PDFOutlineFlags) -> int:
        """
        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> parent_id (int)\n
        The id of the parent item or `PDF_OUTLINE_ROOT` if this is a top level item.
        >>> utf8 (str)\n
        The name of the outline.
        >>> link_attribs (str)\n
        The link attributes specifying where this outline links to.
        >>> flags (cairo.PDFOutlineFlags)\n
        Outline item flags.

        Returns
        -------
        >>> int\n
        The ID for the added item.
        """
    def set_metadata(self, metadata: PDFMetadata, utf8: str) -> None:
        """
        Set document metadata. The `PDFMetadata.CREATE_DATE` and `PDFMetadata.MOD_DATE` values must be in ISO-8601 format: YYYY-MM-DDThh:mm:ss. An optional timezone of the form “[+/-]hh:mm” or “Z” for UTC time can be appended. All other metadata values can be any UTF-8 string.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> metadata (cairo.PDFMetadata)\n
        The metadata item to set.
        >>> utf8 (str)\n
        Metadata value.
        """
    def set_page_label(self, utf8: str) -> None:
        """
        Set page label for the current page.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> utf8 (str)\n
        Metadata value.
        """
    def set_thumbnail_size(self, width: int, height: int) -> None:
        """
        Set the thumbnail image size for the current and all subsequent pages. Setting a width or height of 0 disables thumbnails for the current and subsequent pages.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> width (int)\n
        Thumbnail width.
        >>> height (int)\n
        Thumbnail height.
        """

class PSSurface(Surface):
    """
    The PSSurface is used to render cairo graphics to Adobe PostScript files and is a multi-page vector surface backend.

    Note that the size of individual pages of the PostScript output can vary. See `set_size()`.

    Parameters
    ----------
    >>> fobj (str or None)\n
    (`None`, `pathlike`, file or file-like object) – A filename or writable file object. `None` may be used to specify no output. This will generate a PSSurface that may be queried and used as a source, without generating a temporary file.
    >>> width_in_points (float)\n
    Width of the surface, in points (1 point == 1/72.0 inch).
    >>> height_in_points (float)\n
    Height of the surface, in points (1 point == 1/72.0 inch).

    Returns
    -------
    >>> cairo.PSSurface\n
    A new PSSurface of the specified size in points to be written to fobj.

    Raises
    ------
    >>> cairo.MemoryError\n
    In case of no memory.
    """
    def __init__(self, fobj: Union[_FileLike, _PathLike], width_in_points: float, height_in_points: float) -> None: ...
    def dsc_begin_page_setup(self) -> None:
        """
        This method indicates that subsequent calls to `dsc_comment()` should direct comments to the PageSetup section of the PostScript output.

        This method call is only needed for the first page of a surface. It should be called after any call to `dsc_begin_setup()` and before any drawing is performed to the surface.

        See `dsc_comment()` for more details.

        New in version 1.2.
        """
    def dsc_begin_setup(self) -> None:
        """
        This function indicates that subsequent calls to `dsc_comment()` should direct comments to the Setup section of the PostScript output.

        This function should be called at most once per surface, and must be called before any call to `dsc_begin_page_setup()` and before any drawing is performed to the surface.

        See `dsc_comment()` for more details.

        New in version 1.2.
        """
    def dsc_comment(self, comment: str) -> None:
        """
        Emit a comment into the PostScript output for the given surface.

        The comment is expected to conform to the PostScript Language Document Structuring Conventions (DSC). Please see that manual for details on the available comments and their meanings. In particular, the %%IncludeFeature comment allows a device-independent means of controlling printer device features. So the PostScript Printer Description Files Specification will also be a useful reference.

        The comment string must begin with a percent character (%) and the total length of the string (including any initial percent characters) must not exceed 255 characters. Violating either of these conditions will place PSSurface into an error state. But beyond these two conditions, this function will not enforce conformance of the comment with any particular specification.

        The comment string should not have a trailing newline.

        The DSC specifies different sections in which particular comments can appear. This function provides for comments to be emitted within three sections: the header, the Setup section, and the PageSetup section. Comments appearing in the first two sections apply to the entire document while comments in the BeginPageSetup section apply only to a single page.

        For comments to appear in the header section, this function should be called after the surface is created, but before a call to `dsc_begin_setup()`.

        For comments to appear in the Setup section, this function should be called after a call to `dsc_begin_setup()` but before a call to `dsc_begin_page_setup()`.

        For comments to appear in the PageSetup section, this function should be called after a call to `dsc_begin_page_setup()`.

        Note that it is only necessary to call `dsc_begin_page_setup()` for the first page of any surface. After a call to `Context.show_page()` or `Context.copy_page()` comments are unambiguously directed to the PageSetup section of the current page. But it doesn’t hurt to call this function at the beginning of every page as that consistency may make the calling code simpler.

        As a final note, cairo automatically generates several comments on its own. As such, applications must not manually generate any of the following comments:

        Header section: %!PS-Adobe-3.0, %Creator, %CreationDate, %Pages, %BoundingBox, %DocumentData, %LanguageLevel, %EndComments.

        Setup section: %BeginSetup, %EndSetup

        PageSetup section: %BeginPageSetup, %PageBoundingBox, %EndPageSetup.

        Other sections: %BeginProlog, %EndProlog, %Page, %Trailer, %EOF

        Here is an example sequence showing how this function might be used:
        >>> surface = PSSurface (filename, width, height)
        >>> ...
        >>> surface.dsc_comment (surface, "%%Title: My excellent document")
        >>> surface.dsc_comment (surface, "%%Copyright: Copyright (C) 2006 Cairo Lover")
        >>> ...
        >>> surface.dsc_begin_setup (surface)
        >>> surface.dsc_comment (surface, "%%IncludeFeature: *MediaColor White")
        >>> ...
        >>> surface.dsc_begin_page_setup (surface)
        >>> surface.dsc_comment (surface, "%%IncludeFeature: *PageSize A3")
        >>> surface.dsc_comment (surface, "%%IncludeFeature: *InputSlot LargeCapacity")
        >>> surface.dsc_comment (surface, "%%IncludeFeature: *MediaType Glossy")
        >>> surface.dsc_comment (surface, "%%IncludeFeature: *MediaColor Blue")
        >>> ... draw to first page here ..
        >>> ctx.show_page (cr)
        >>> ...
        >>> surface.dsc_comment (surface, "%%IncludeFeature:  PageSize A5");
        >>> ...\n

        New in version 1.2.

        Parameters
        ----------
        >>> comment (str)\n
        A comment string to be emitted into the PostScript output.
        """
    def get_eps(self) -> bool:
        """
        New in version 1.6.

        Returns
        -------
        >>> bool\n
        Returns `True` if the `PSSurface` will output Encapsulated PostScript.
        """
    @staticmethod
    def level_to_string(level: PSLevel) -> str:
        """
        Get the string representation of the given level. See `get_levels()` for a way to get the list of valid level ids.

        Note
        ----
        Prior to 1.12 this was available under `ps_level_to_string()`.

        Parameters
        ----------
        >>> level (cairo.PSLevel)\n
        A `PSLevel`.

        Returns
        -------
        >>> str\n
        The string associated to given level.
        """
    ps_level_to_string = level_to_string
    def restrict_to_level(self, level: PSLevel) -> None:
        """
        Restricts the generated PostSript file to level. See `get_levels()` for a list of available level values that can be used here.

        This function should only be called before any drawing operations have been performed on the given surface. The simplest way to do this is to call this function immediately after creating the surface.

        New in version 1.6.

        Parameters
        ----------
        >>> level (cairo.PSLevel)\n
        A `PSLevel`.
        """
    def set_eps(self, eps: bool) -> None:
        """
        If `eps` is `True`, the PostScript surface will output Encapsulated PostScript.

        This function should only be called before any drawing operations have been performed on the current page. The simplest way to do this is to call this function immediately after creating the surface. An Encapsulated PostScript file should never contain more than one page.

        New in version 1.6.

        Parameters
        ----------
        >>> eps (bool)\n
        `True` to output EPS format PostScript.
        """
    def set_size(self, width_in_points: float, height_in_points: float) -> None:
        """
        Changes the size of a PostScript surface for the current (and subsequent) pages.

        This function should only be called before any drawing operations have been performed on the current page. The simplest way to do this is to call this function immediately after creating the surface or immediately after completing a page with either `Context.show_page()` or `Context.copy_page()`.

        New in version 1.2.

        Parameters
        ----------
        >>> width_in_points (float)\n
        New surface width, in points (1 point == 1/72.0 inch).
        >>> height_in_points (float)\n
        New surface height, in points (1 point == 1/72.0 inch).
        """
    @staticmethod
    def get_levels() -> List[PSLevel]:
        """
        Retrieve the list of supported levels. See `restrict_to_level()`.

        New in version 1.12.0.

        Returns
        -------
        >>> list(cairo.PSLevel)\n
        Supported level list.
        """

class SVGSurface(Surface):
    """
    The SVGSurface is used to render cairo graphics to SVG files and is a multi-page vector surface backend.

    Parameters
    ----------
    >>> fobj (str or None)\n
    (`None`, `pathlike`, file or file-like object) – A filename or writable file object. `None` may be used to specify no output. This will generate a `SVGSurface` that may be queried and used as a source, without generating a temporary file.
    >>> width_in_points (float)\n
    Width of the surface, in points (1 point == 1/72.0 inch).
    >>> height_in_points (float)\n
    Height of the surface, in points (1 point == 1/72.0 inch).

    Returns
    -------
    >>> cairo.SVGSurface\n
    A new `SVGSurface` of the specified size in points to be written to `fobj`.

    Raises
    ------
    >>> cairo.MemoryError\n
    In case of no memory.
    """
    def __init__(self, fobj: Union[_PathLike, _FileLike], width_in_points: float, height_in_points: float) -> None: ...
    def restrict_to_version(self, version: SVGVersion) -> None:
        """
        Restricts the generated SVG file to version . See `get_versions()` for a list of available version values that can be used here.

        This function should only be called before any drawing operations have been performed on the given surface. The simplest way to do this is to call this function immediately after creating the surface.

        New in version 1.12.0.

        Parameters
        ----------
        >>> version (cairo.SVGVersion)\n
        """
    @staticmethod
    def get_versions() -> List[SVGVersion]:
        """
        Retrieve the list of supported versions. See `restrict_to_version()`.

        New in version 1.12.0.

        Returns
        -------
        >>> list(cairo.SVGVersion)\n
        Supported version list.
        """
    @staticmethod
    def version_to_string(version: SVGVersion) -> str:
        """
        Get the string representation of the given version ID. See `get_versions()` for a way to get the list of valid version IDs.

        New in version 1.12.0.

        Parameters
        ----------
        >>> version (cairo.SVGVersion)\n

        Returns
        -------
        >>> str\n
        The string associated to the given version.

        Raises
        ------
        >>> ValueError\n
        If the version isn't valid.
        """
    def get_document_unit(self) -> SVGUnit:
        """
        Get the unit of the `SVGSurface`.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Returns
        -------
        >>> cairo.SVGUnit\n
        The SVG unit of the `SVGSurface`.
        """
    def set_document_unit(self, unit: SVGUnit) -> None:
        """
        Use the specified unit for the width and height of the generated SVG file. See `SVGUnit` for a list of available unit values that can be used here.

        This function can be called at any time before generating the SVG file.

        However to minimize the risk of ambiguities it’s recommended to call it before any drawing operations have been performed on the given surface, to make it clearer what the unit used in the drawing operations is.

        The simplest way to do this is to call this function immediately after creating the SVG surface.

        Note if this function is never called, the default unit for SVG documents generated by cairo will be “pt”. This is for historical reasons.

        New in version 1.18.0: Only available with cairo 1.15.10+.

        Parameters
        ----------
        >>> unit (cairo.SVGUnit)\n
        """

class RadialGradient(Gradient):
    """
    Creates a new `RadialGradient` pattern between the two circles defined by (cx0, cy0, radius0) and (cx1, cy1, radius1). Before using the gradient pattern, a number of color stops should be defined using `Gradient.add_color_stop_rgb()` or `Gradient.add_color_stop_rgba()`.

    Note: The coordinates here are in pattern space. For a new pattern, pattern space is identical to user space, but the relationship between the spaces can be changed with `Pattern.set_matrix()`.

    Parameters
    ----------
    >>> cx0 (float)\n
    X coordinate for the center of the start circle.
    >>> cy0 (float)\n
    Y coordinate for the center of the start circle.
    >>> radius0 (float)\n
    Radius of the start circle.
    >>> cx1 (float)\n
    X coordinate for the center of the end circle.
    >>> cy1 (float)\n
    Y coordinate for the center of the end circle.
    >>> radius1 (float)
    Radius of the end circle.

    Returns
    -------
    >>> RadialGradient\n
    The newly created `RadialGradient`.

    Raises
    ------
    >>> MemoryError\n
    In case of no memory.
    """
    def __init__(self, cx0: float, cy0: float, radius0: float, cx1: float, cy1: float, radius1: float) -> None: ...
    def get_radial_circles(self) -> Tuple[float, float, float, float, float, float]:
        """
        Gets the Gradient endpoint circles for a RadialGradient, each specified as a center coordinate and a radius.

        New in version 1.4.

        Returns
        -------
        >>> (x0: float, y0: float, r0: float, x1: float, y1: float, r1: float)\n
        - `x0 (float)` - Return value for the x coordinate of the center of the first circle.
        - `y0 (float)` - Return value for the y coordinate of the center of the first circle.
        - `r0 (float)` - Return value for the radius of the first circle.
        - `x1 (float)` - Return value for the x coordinate of the center of the second circle.
        - `y1 (float)` - Return value for the y coordinate of the center of the second circle.
        - `r1 (float)` - Return value for the radius of the second circle.
        """

_AcquireCallback = Callable[[Surface, RectangleInt], Surface]
_ReleaseCallback = Callable[[Surface], None]

class RasterSourcePattern(Pattern):
    """
    Creates a new user pattern for providing pixel data.

    Use the setter functions to associate callbacks with the returned pattern.

    New in version 1.15.

    The raster source provides the ability to supply arbitrary pixel data whilst rendering. The pixels are queried at the time of rasterisation by means of user callback functions, allowing for the ultimate flexibility. For example, in handling compressed image sources, you may keep a MRU cache of decompressed images and decompress sources on the fly and discard old ones to conserve memory.

    For the raster source to be effective, you must at least specify the acquire and release callbacks which are used to retrieve the pixel data for the region of interest and demark when it can be freed afterwards. Other callbacks are provided for when the pattern is copied temporarily during rasterisation, or more permanently as a snapshot in order to keep the pixel data available for printing.

    Parameters
    ----------
    >>> content (Content)\n
    Content type for the pixel data that will be returned. Knowing the content type ahead of time is used for analysing the operation and picking the appropriate rendering path.
    >>> width (int)\n
    Maximum size of the sample area.
    >>> height (int)\n
    Maximum size of the sample area.

    Returns
    -------
    >>> cairo.RasterSourcePattern\n
    A newly created `RasterSourcePattern`.

    Raises
    ------
    >>> cairo.Error
    """
    def __init__(self, content: Content, width: int, height: int) -> None: ...
    def set_acquire(self, acquire: Optional[_AcquireCallback], release: Optional[_ReleaseCallback]) -> None: ...
    def get_acquire(self) -> Tuple[Optional[_AcquireCallback], Optional[_ReleaseCallback]]:
        """
        Queries the current acquire and release callbacks.

        New in version 1.15.

        Returns
        -------
        >>> (acquire: Optional[callable], release: Optional[callable])\n
        A tuple of callables or `None` as set through `set_acquire()`.
        """

class RecordingSurface(Surface):
    """
    Creates a RecordingSurface which can be used to record all drawing operations at the highest level (that is, the level of paint, mask, stroke, fill and show_text_glyphs). The RecordingSurface can then be “replayed” against any target surface by using it as a source to drawing operations.

    The recording phase of the RecordingSurface is careful to snapshot all necessary objects (paths, patterns, etc.), in order to achieve accurate replay.

    New in version 1.11.0.

    A RecordingSurface is a surface that records all drawing operations at the highest level of the surface backend interface, (that is, the level of paint, mask, stroke, fill, and show_text_glyphs). The recording surface can then be “replayed” against any target surface by using it as a source surface.

    If you want to replay a surface so that the results in target will be identical to the results that would have been obtained if the original operations applied to the recording surface had instead been applied to the target surface, you can use code like this:
    >>> cr = cairo.Context(target)
    >>> cr.set_source_surface(recording_surface, 0.0, 0.0)
    >>> cr.paint()\n

    A RecordingSurface is logically unbounded, i.e. it has no implicit constraint on the size of the drawing surface. However, in practice this is rarely useful as you wish to replay against a particular target surface with known bounds. For this case, it is more efficient to specify the target extents to the recording surface upon creation.

    The recording phase of the recording surface is careful to snapshot all necessary objects (paths, patterns, etc.), in order to achieve accurate replay.

    Parameters
    ----------
    >>> content (cairo.Content)\n
    The content for the new surface.
    >>> rectangle (cairo.Rectangle or None)\n
    `Rectangle` that bound the recorded operations, or `None` to record unbounded operations.
    """
    def __init__(self, content: Content, rectangle: Rectangle) -> None: ...
    def ink_extents(self) -> Tuple[float, float, float, float]:
        """
        Measures the extents of the operations stored within the `RecordingSurface`. This is useful to compute the required size of an `ImageSurface` (or equivalent) into which to replay the full sequence of drawing operations.

        New in version 1.11.0.

        Returns
        -------
        >>> x0 (float)\n
        The X coordinate of the top-left of the ink bounding box.
        >>> y0 (float)\n
        The Y coordinate of the top-left of the ink bounding box.
        >>> width (float)\n
        The width of the ink bounding box.
        >>> height (float)\n
        The height of the ink bounding box.
        """
    def get_extents(self) -> Optional[Rectangle]:
        """
        Get the extents of the recording-surface.

        New in version 1.12.0.

        Returns
        -------
        >>> cairo.Rectangle or None\n
        A `Rectangle` or `None` if the surface is unbounded.
        """

class Region:
    """
    Allocates a new empty region object or a region object with the containing rectangle(s).

    Region is a simple graphical data type representing an area of integer-aligned rectangles. They are often used on raster surfaces to track areas of interest, such as change or clip areas.

    .. versionadded:: 1.11.0
    """

    def __init__(self, rectangle: Union[RectangleInt, List[RectangleInt]]) -> None:
        """
        :param rectangle_int: a rectangle or a list of rectangle
        """

    def copy(self) -> "Region":
        """
        :returns: A newly allocated :class:`Region`.

        Allocates a new *Region* object copying the area from original.
        """

    def get_extents(self) -> RectangleInt:
        """
        :returns: The bounding rectangle of region
        """

    def num_rectangles(self) -> int:
        """
        :returns: The number of rectangles contained in region
        """

    def get_rectangle(self, nth: int) -> RectangleInt:
        """
        :param nth: a number indicating which rectangle should be returned
        :returns: The *nth* rectangle from the region
        """

    def is_empty(self) -> bool:
        """
        :returns: Whether region is empty
        """

    def contains_point(self, x: int, y: int) -> bool:
        """
        :param x: The x coordinate of a point
        :param y: The y coordinate of a point
        :returns: Whether (x , y) is contained in the region
        """

    def contains_rectangle(self, rectangle: RectangleInt) -> RegionOverlap:
        """
        :param rectangle: a region
        :returns: region overlap

        Checks whether rectangle is inside, outside or partially contained in
        region
        """

    def equal(self, region: "Region") -> bool:
        """
        :param region: a region
        :returns: Whether both regions contained the same coverage
        """

    def translate(self, dx: int, dy: int) -> None:
        """
        :param dx: Amount to translate in the x direction
        :param dy: Amount to translate in the y direction

        Translates region by (dx , dy ).
        """

    def intersect(self, other: "Union[Region, RectangleInt]") -> "Region":
        """
        :param other: region or rectangle
        :returns: The intersection of the region and the passed region or
            rectangle
        """

    def subtract(self, other: "Union[Region, RectangleInt]") -> "Region":
        """
        :param other: region or rectangle
        :returns: The result of the subtraction of the region and the passed
            region or rectangle
        """

    def union(self, other: "Union[Region, RectangleInt]") -> "Region":
        """
        :param other: region or rectangle
        :returns: The union of the region and the passed region or rectangle
        """

    def xor(self, other: "Union[Region, RectangleInt]") -> "Region":
        """
        :param other: region or rectangle
        :returns: The exclusive difference of the region and the passed region
            or rectangle
        """

class ScriptDevice(Device):
    """
    Creates a output device for emitting the script, used when creating the individual surfaces.

    New in version 1.14.

    Parameters
    ----------
    >>> fobj (str)\n
    (pathlike, file or file-like object) – A filename or writable file object.
    """
    def __init__(self, fobj: Union[_FileLike, _PathLike]) -> None: ...
    def set_mode(self, mode: ScriptMode) -> None:
        """
        Change the output mode of the script.

        Parameters
        ----------
        >>> mode (cairo.ScriptMode)\n
        The new mode.
        """
    def get_mode(self) -> ScriptMode:
        """
        Queries the script for its current output mode.

        Returns
        -------
        >>> cairo.ScriptMode\n
        The current output mode of the script.
        """
    def write_comment(self, comment: str) -> None:
        """
        Emit a string verbatim into the script.

        Parameters
        ----------
        >>> comment (str)\n
        The string to emit.
        """
    def from_recording_surface(self, recording_surface: RecordingSurface) -> None:
        """
        Converts the record operations in `recording_surface` into a script.

        Parameters
        ----------
        >>> recording_surface (cairo.RecordingSurface)\n
        The recording surface to replay.

        Raises
        ------
        >>> cairo.Error
        """

class ScriptSurface(Surface):
    """
    Create a new surface that will emit its rendering through `script`.

    New in version 1.14.

    The script surface provides the ability to render to a native script that matches the cairo drawing model. The scripts can be replayed using tools under the util/cairo-script directory, or with cairo-perf-trace.

    Parameters
    ----------
    >>> script (cairo.ScriptDevice)\n
    The script (output device).
    >>> content (cairo.Content)\n
    The content of the surface.
    >>> width (float)\n
    Width in pixels.
    >>> height (float)\n
    Height in pixels.

    Returns
    -------
    >>> cairo.ScriptSurface\n
    A newly created `ScriptSurface`.

    Raises
    ------
    >>> cairo.Error\n
    """
    def __init__(self, script: ScriptDevice, content: Content, width: float, height: float) -> None: ...
    @classmethod
    def create_for_target(cls, script: ScriptDevice, target: Surface) -> "ScriptSurface":
        """
        Create a proxy surface that will render to `target` and record the operations to `device`.

        New in version 1.14

        Parameters
        ----------
        >>> script (cairo.ScriptDevice)\n
        The script (output device).
        >>> target (cairo.Surface)\n
        The target surface to wrap.
        """

class Win32Surface(Surface):
    """
    Creates a cairo surface that targets the given DC. The DC will be queried for its initial clip extents, and this will be used as the size of the cairo surface. The resulting surface will always be of format `cairo.FORMAT_RGB24`, see `cairo.Format`.

    The Microsoft Windows surface is used to render cairo graphics to Microsoft Windows windows, bitmaps, and printing device contexts.

    Parameters
    ----------
    >>> hdc (int)\n
    The DC to create a surface for.

    Returns
    -------
    >>> cairo.Win32Surface\n
    A newly created surface.
    """
    def __init__(self, hdc: int) -> None: ...

class Win32PrintingSurface(Surface):
    """
    Creates a cairo surface that targets the given DC. The DC will be queried for its initial clip extents, and this will be used as the size of the cairo surface. The DC should be a printing DC; antialiasing will be ignored, and GDI will be used as much as possible to draw to the surface.

    The returned surface will be wrapped using the paginated surface to provide correct complex rendering behaviour; `cairo.Surface.show_page()` and associated methods must be used for correct output.

    The Win32PrintingSurface is a multi-page vector surface type.

    Parameters
    ----------
    >>> hdc (int)\n
    The DC to create a surface for.

    Returns
    -------
    >>> cairo.Win32PrintingSurface\n
    A newly created surface.
    """
    def __init__(self, hdc: int) -> None: ...

class SolidPattern(Pattern):
    """
    Creates a new SolidPattern corresponding to a translucent color. The color components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.

    Parameters
    ----------
    >>> red (float)\n
    Red component of the color.
    >>> green (float)\n
    Green component of the color.
    >>> blue (float)\n
    Blue component of the color.
    >>> alpha (float)\n
    Alpha component of the color

    Returns
    -------
    A new `SolidPattern`.

    Raises
    ------
    >>> MemoryError\n
    In case of no memory.
    """
    def __init__(self, red: float, green: float, blue: float, alpha: float=1.0) -> None: ...
    def get_rgba(self) -> Tuple[float, float, float, float]:
        """
        Gets the solid color for a `SolidPattern`.

        New in version 1.4.

        Returns
        -------
        >>> (red: float, green: float, blue: float, alpha: float)\n
        """

class SurfaceObserverMode(_IntEnum):
    """
    Whether operations should be recorded.

    New in version 1.14.
    """
    NORMAL: "SurfaceObserverMode" = ...
    RECORD_OPERATIONS: "SurfaceObserverMode" = ...

class TeeSurface(Surface):
    """
    This surface supports redirecting all its input to multiple surfaces.

    New in version 1.14.

    Parameters
    ----------
    >>> master (cairo.Surface)\n

    Returns
    -------
    >>> cairo.TeeSurface\n

    Raises
    ------
    >>> cairo.Error\n
    """
    def __init__(self, master: Surface) -> None: ...
    def add(self, target: Surface) -> None:
        """
        Adds the surface.

        New in version 1.14.

        Parameters
        ----------
        >>> target (cairo.Surface)\n

        Raises
        ------
        >>> cairo.Error\n
        """
    def remove(self, target: Surface) -> None:
        """
        Removes the surface.

        New in version 1.14.

        Parameters
        ----------
        >>> target (cairo.Surface)\n
        
        Raises
        ------
        >>> cairo.Error\n
        """
    def index(self, index: int) -> Surface:
        """
        Returns the surface at index `index`. The master surface is at index 0.

        New in version 1.14.

        Parameters
        ----------
        >>> index (int)\n

        Returns
        -------
        >>> cairo.Error\n
        """

class ToyFontFace(FontFace):
    """
    The cairo.ToyFontFace class can be used instead of Context.select_font_face() to create a toy font independently of a context.

    Creates a ToyFontFace from a triplet of family, slant, and weight. These font faces are used in implementation of the the “toy” font API.

    If family is the zero-length string “”, the platform-specific default family is assumed. The default family then can be queried using `get_family()`.

    The `Context.select_font_face()` method uses this to create font faces. See that function for limitations of toy font faces.

    New in version 1.8.4.

    Parameters
    ----------
    >>> family (str)\n
    A font family name.
    >>> slant (cairo.FontSlant)\n
    The font slant of the font, defaults to `cairo.FontSlant.NORMAL`.
    >>> weight (cairo.FontWeight)\n
    The font weight of the font, defaults to `cairo.FontWeight.NORMAL`.

    Returns
    -------
    >>> cairo.ToyFontFace\n
    A newly created `ToyFontFace` instance.
    """
    def __init__(self, family: str, slant: FontSlant=..., weight: FontWeight=...) -> None: ...
    def get_family(self) -> str:
        """
        Returns
        -------
        >>> str\n
        The family name of a toy font.
        """
    def get_slant(self) -> FontSlant:
        """
        New in version 1.8.4

        Returns
        -------
        >>> cairo.FontSlant\n
        The font slant value.
        """
    def get_weight(self) -> FontWeight:
        """
        New in version 1.8.4

        Returns
        -------
        >>> cairo.FontWeight\n
        The font weight value.
        """

class XCBSurface(Surface):
    """
    Creates a cairo surface that targets the given drawable (pixmap or window).

    The XCB surface is used to render cairo graphics to X Window System windows and pixmaps using the XCB library.

    Note that the XCB surface automatically takes advantage of the X render extension if it is available.

    Note
    ----
    This type isn't implemented. Please file a bug if you need it.

    Parameters
    ----------
    >>> connection (cairo.XCBConnection) #type not implemented\n
    A `XCBConnection`.
    >>> drawable (cairo.XDrawable) #type not implemented\n
    A `XDrawable`.
    >>> visualtype (cairo.XVisualType) #type not implemented\n
    A `XVisualType`.
    >>> width (int)\n
    The surface width.
    >>> height (int)\n
    The surface height.
    """
    def __init__(self, connection: Any, drawable: Any, visualtype: Any, width: int, height: int) -> None: ...
    def set_size(self, width: int, height: int) -> None:
        """
        Informs cairo of the new size of the X Drawable underlying the surface. For a surface created for a Window (rather than a Pixmap), this function must be called each time the size of the window changes. (For a subwindow, you are normally resizing the window yourself, but for a toplevel window, it is necessary to listen for ConfigureNotify events.)

        A Pixmap can never change size, so it is never necessary to call this function on a surface created for a Pixmap.

        Parameters
        ----------
        >>> width (int)\n
        The width of the surface.
        >>> height (int)\n
        The height of the surface.
        """

class XlibSurface(Surface):
    """
    The XLib surface is used to render cairo graphics to X Window System windows and pixmaps using the XLib library.

    Note that the XLib surface automatically takes advantage of X render extension if it is available.

    Note
    ----
    `XlibSurface` cannot be instantiated directly because Python interaction with Xlib would require open source Python bindings to Xlib which provided a C API. However, a `XlibSurface` instance can be returned from a function call when using pygtk http://www.pygtk.org/.
    """
    def get_depth(self) -> int:
        """
        New in version 1.2.

        Returns
        -------
        >>> int\n
        The number of bits used to represent each pixel value.
        """
    def get_height(self) -> int:
        """
        New in version 1.2.

        Returns
        -------
        >>> int\n
        The height of the X Drawable underlying the surface in pixels.
        """
    def get_width(self) -> int:
        """
        New in version 1.2.

        Returns
        -------
        >>> int\n
        The width of the X Drawable underlying the surface in pixels.
        """

def get_include() -> _PathLike:
    """
    Gives the include path which should be passed to the compiler.

    New in version 1.16.0.

    Returns
    -------
    >>> str\n
    A path to the directory containing the C header files.
    """

MIME_TYPE_JP2: str = ...
MIME_TYPE_JPEG: str = ...
MIME_TYPE_PNG: str = ...
MIME_TYPE_URI: str = ...
MIME_TYPE_UNIQUE_ID: str = ...
MIME_TYPE_CCITT_FAX: str = ...
MIME_TYPE_CCITT_FAX_PARAMS: str = ...
MIME_TYPE_EPS: str = ...
MIME_TYPE_EPS_PARAMS: str = ...
MIME_TYPE_JBIG2: str = ...
MIME_TYPE_JBIG2_GLOBAL: str = ...
MIME_TYPE_JBIG2_GLOBAL_ID: str = ...
TAG_DEST: str = ...
TAG_LINK: str = ...

CAPI: Any = ...

ANTIALIAS_BEST = Antialias.BEST
ANTIALIAS_DEFAULT = Antialias.DEFAULT
ANTIALIAS_FAST = Antialias.FAST
ANTIALIAS_GOOD = Antialias.GOOD
ANTIALIAS_GRAY = Antialias.GRAY
ANTIALIAS_NONE = Antialias.NONE
ANTIALIAS_SUBPIXEL = Antialias.SUBPIXEL
CONTENT_ALPHA = Content.ALPHA
CONTENT_COLOR = Content.COLOR
CONTENT_COLOR_ALPHA = Content.COLOR_ALPHA
EXTEND_NONE = Extend.NONE
EXTEND_PAD = Extend.PAD
EXTEND_REFLECT = Extend.REFLECT
EXTEND_REPEAT = Extend.REPEAT
FILL_RULE_EVEN_ODD = FillRule.EVEN_ODD
FILL_RULE_WINDING = FillRule.WINDING
FILTER_BEST = Filter.BEST
FILTER_BILINEAR = Filter.BILINEAR
FILTER_FAST = Filter.FAST
FILTER_GAUSSIAN = Filter.GAUSSIAN
FILTER_GOOD = Filter.GOOD
FILTER_NEAREST = Filter.NEAREST
FONT_SLANT_ITALIC = FontSlant.ITALIC
FONT_SLANT_NORMAL = FontSlant.NORMAL
FONT_SLANT_OBLIQUE = FontSlant.OBLIQUE
FONT_WEIGHT_BOLD = FontWeight.BOLD
FONT_WEIGHT_NORMAL = FontWeight.NORMAL
FORMAT_A1 = Format.A1
FORMAT_A8 = Format.A8
FORMAT_ARGB32 = Format.ARGB32
FORMAT_INVALID = Format.INVALID
FORMAT_RGB16_565 = Format.RGB16_565
FORMAT_RGB24 = Format.RGB24
FORMAT_RGB30 = Format.RGB30
HINT_METRICS_DEFAULT = HintMetrics.DEFAULT
HINT_METRICS_OFF = HintMetrics.OFF
HINT_METRICS_ON = HintMetrics.ON
HINT_STYLE_DEFAULT = HintStyle.DEFAULT
HINT_STYLE_FULL = HintStyle.FULL
HINT_STYLE_MEDIUM = HintStyle.MEDIUM
HINT_STYLE_NONE = HintStyle.NONE
HINT_STYLE_SLIGHT = HintStyle.SLIGHT
LINE_CAP_BUTT = LineCap.BUTT
LINE_CAP_ROUND = LineCap.ROUND
LINE_CAP_SQUARE = LineCap.SQUARE
LINE_JOIN_BEVEL = LineJoin.BEVEL
LINE_JOIN_MITER = LineJoin.MITER
LINE_JOIN_ROUND = LineJoin.ROUND
OPERATOR_ADD = Operator.ADD
OPERATOR_ATOP = Operator.ATOP
OPERATOR_CLEAR = Operator.CLEAR
OPERATOR_COLOR_BURN = Operator.COLOR_BURN
OPERATOR_COLOR_DODGE = Operator.COLOR_DODGE
OPERATOR_DARKEN = Operator.DARKEN
OPERATOR_DEST = Operator.DEST
OPERATOR_DEST_ATOP = Operator.DEST_ATOP
OPERATOR_DEST_IN = Operator.DEST_IN
OPERATOR_DEST_OUT = Operator.DEST_OUT
OPERATOR_DEST_OVER = Operator.DEST_OVER
OPERATOR_DIFFERENCE = Operator.DIFFERENCE
OPERATOR_EXCLUSION = Operator.EXCLUSION
OPERATOR_HARD_LIGHT = Operator.HARD_LIGHT
OPERATOR_HSL_COLOR = Operator.HSL_COLOR
OPERATOR_HSL_HUE = Operator.HSL_HUE
OPERATOR_HSL_LUMINOSITY = Operator.HSL_LUMINOSITY
OPERATOR_HSL_SATURATION = Operator.HSL_SATURATION
OPERATOR_IN = Operator.IN
OPERATOR_LIGHTEN = Operator.LIGHTEN
OPERATOR_MULTIPLY = Operator.MULTIPLY
OPERATOR_OUT = Operator.OUT
OPERATOR_OVER = Operator.OVER
OPERATOR_OVERLAY = Operator.OVERLAY
OPERATOR_SATURATE = Operator.SATURATE
OPERATOR_SCREEN = Operator.SCREEN
OPERATOR_SOFT_LIGHT = Operator.SOFT_LIGHT
OPERATOR_SOURCE = Operator.SOURCE
OPERATOR_XOR = Operator.XOR
PATH_CLOSE_PATH = PathDataType.CLOSE_PATH
PATH_CURVE_TO = PathDataType.CURVE_TO
PATH_LINE_TO = PathDataType.LINE_TO
PATH_MOVE_TO = PathDataType.MOVE_TO
PDF_VERSION_1_4 = PDFVersion.VERSION_1_4
PDF_VERSION_1_5 = PDFVersion.VERSION_1_5
PS_LEVEL_2 = PSLevel.LEVEL_2
PS_LEVEL_3 = PSLevel.LEVEL_3
REGION_OVERLAP_IN = RegionOverlap.IN
REGION_OVERLAP_OUT = RegionOverlap.OUT
REGION_OVERLAP_PART = RegionOverlap.PART
SCRIPT_MODE_ASCII = ScriptMode.ASCII
SCRIPT_MODE_BINARY = ScriptMode.BINARY
STATUS_CLIP_NOT_REPRESENTABLE = Status.CLIP_NOT_REPRESENTABLE
STATUS_DEVICE_ERROR = Status.DEVICE_ERROR
STATUS_DEVICE_FINISHED = Status.DEVICE_FINISHED
STATUS_DEVICE_TYPE_MISMATCH = Status.DEVICE_TYPE_MISMATCH
STATUS_FILE_NOT_FOUND = Status.FILE_NOT_FOUND
STATUS_FONT_TYPE_MISMATCH = Status.FONT_TYPE_MISMATCH
STATUS_INVALID_CLUSTERS = Status.INVALID_CLUSTERS
STATUS_INVALID_CONTENT = Status.INVALID_CONTENT
STATUS_INVALID_DASH = Status.INVALID_DASH
STATUS_INVALID_DSC_COMMENT = Status.INVALID_DSC_COMMENT
STATUS_INVALID_FORMAT = Status.INVALID_FORMAT
STATUS_INVALID_INDEX = Status.INVALID_INDEX
STATUS_INVALID_MATRIX = Status.INVALID_MATRIX
STATUS_INVALID_MESH_CONSTRUCTION = Status.INVALID_MESH_CONSTRUCTION
STATUS_INVALID_PATH_DATA = Status.INVALID_PATH_DATA
STATUS_INVALID_POP_GROUP = Status.INVALID_POP_GROUP
STATUS_INVALID_RESTORE = Status.INVALID_RESTORE
STATUS_INVALID_SIZE = Status.INVALID_SIZE
STATUS_INVALID_SLANT = Status.INVALID_SLANT
STATUS_INVALID_STATUS = Status.INVALID_STATUS
STATUS_INVALID_STRIDE = Status.INVALID_STRIDE
STATUS_INVALID_STRING = Status.INVALID_STRING
STATUS_INVALID_VISUAL = Status.INVALID_VISUAL
STATUS_INVALID_WEIGHT = Status.INVALID_WEIGHT
STATUS_JBIG2_GLOBAL_MISSING = Status.JBIG2_GLOBAL_MISSING
STATUS_LAST_STATUS = Status.LAST_STATUS
STATUS_NEGATIVE_COUNT = Status.NEGATIVE_COUNT
STATUS_NO_CURRENT_POINT = Status.NO_CURRENT_POINT
STATUS_NO_MEMORY = Status.NO_MEMORY
STATUS_NULL_POINTER = Status.NULL_POINTER
STATUS_PATTERN_TYPE_MISMATCH = Status.PATTERN_TYPE_MISMATCH
STATUS_READ_ERROR = Status.READ_ERROR
STATUS_SUCCESS = Status.SUCCESS
STATUS_SURFACE_FINISHED = Status.SURFACE_FINISHED
STATUS_SURFACE_TYPE_MISMATCH = Status.SURFACE_TYPE_MISMATCH
STATUS_TEMP_FILE_ERROR = Status.TEMP_FILE_ERROR
STATUS_USER_FONT_ERROR = Status.USER_FONT_ERROR
STATUS_USER_FONT_IMMUTABLE = Status.USER_FONT_IMMUTABLE
STATUS_USER_FONT_NOT_IMPLEMENTED = Status.USER_FONT_NOT_IMPLEMENTED
STATUS_WRITE_ERROR = Status.WRITE_ERROR
SUBPIXEL_ORDER_BGR = SubpixelOrder.BGR
SUBPIXEL_ORDER_DEFAULT = SubpixelOrder.DEFAULT
SUBPIXEL_ORDER_RGB = SubpixelOrder.RGB
SUBPIXEL_ORDER_VBGR = SubpixelOrder. VBGR
SUBPIXEL_ORDER_VRGB = SubpixelOrder.VRGB
SURFACE_OBSERVER_NORMAL = SurfaceObserverMode.NORMAL
SURFACE_OBSERVER_RECORD_OPERATIONS = SurfaceObserverMode.RECORD_OPERATIONS
SVG_VERSION_1_1 = SVGVersion.VERSION_1_1
SVG_VERSION_1_2 = SVGVersion.VERSION_1_2
TEXT_CLUSTER_FLAG_BACKWARD = TextClusterFlags.BACKWARD
PDF_METADATA_TITLE = PDFMetadata.TITLE
PDF_METADATA_AUTHOR = PDFMetadata.AUTHOR
PDF_METADATA_SUBJECT = PDFMetadata.SUBJECT
PDF_METADATA_KEYWORDS = PDFMetadata.KEYWORDS
PDF_METADATA_CREATOR = PDFMetadata.CREATOR
PDF_METADATA_CREATE_DATE = PDFMetadata.CREATE_DATE
PDF_METADATA_MOD_DATE = PDFMetadata.MOD_DATE
SVG_UNIT_USER = SVGUnit.USER
SVG_UNIT_EM = SVGUnit.EM
SVG_UNIT_EX = SVGUnit.EX
SVG_UNIT_PX = SVGUnit.PX
SVG_UNIT_IN = SVGUnit.IN
SVG_UNIT_CM = SVGUnit.CM
SVG_UNIT_MM = SVGUnit.MM
SVG_UNIT_PT = SVGUnit.PT
SVG_UNIT_PC = SVGUnit.PC
SVG_UNIT_PERCENT = SVGUnit.PERCENT
STATUS_TAG_ERROR = Status.TAG_ERROR
STATUS_FREETYPE_ERROR = Status.FREETYPE_ERROR
STATUS_WIN32_GDI_ERROR = Status.WIN32_GDI_ERROR
STATUS_PNG_ERROR = Status.PNG_ERROR
PDF_OUTLINE_FLAG_OPEN = PDFOutlineFlags.OPEN
PDF_OUTLINE_FLAG_BOLD = PDFOutlineFlags.BOLD
PDF_OUTLINE_FLAG_ITALIC = PDFOutlineFlags.ITALIC
