import pickle
import re

import pytest
import cairo


def test_type() -> None:
    t = cairo.Antialias
    assert int in t.__mro__
    assert isinstance(t(42), int)
    assert isinstance(t(0), int)
    assert issubclass(t, int)

    with pytest.raises(TypeError):
        t()  # type: ignore

    with pytest.raises(TypeError):
        t(object())  # type: ignore

    with pytest.raises(TypeError):
        type("foo", (t,), {})

    assert hasattr(t, "DEFAULT")
    assert t.DEFAULT == 0
    assert t.__name__ == "Antialias"
    assert t.__module__ == "cairo"

    assert repr(t.DEFAULT) == "cairo.Antialias.DEFAULT"
    assert repr(t(123456)) == "123456"
    assert repr(t(0)) == "cairo.Antialias.DEFAULT"
    assert str(t(0)) == "0"

    assert isinstance(cairo.ANTIALIAS_DEFAULT, t)


def test_misc() -> None:
    cairo.Status.JBIG2_GLOBAL_MISSING


def test_format_methods() -> None:
    assert cairo.Format.RGB24.stride_for_width(8) == 32
    assert cairo.Format.stride_for_width(cairo.Format.RGB24, 8) == 32


def test_text_cluster_flags() -> None:
    assert cairo.TextClusterFlags.BACKWARD == 1


def test_surface_observer_mode() -> None:
    assert cairo.SurfaceObserverMode.NORMAL == 0


def test_aliases() -> None:
    types_ = [
        cairo.Antialias,
        cairo.Content,
        cairo.Extend,
        cairo.FillRule,
        cairo.Filter,
        cairo.FontWeight,
        cairo.FontSlant,
        cairo.Format,
        cairo.HintMetrics,
        cairo.HintStyle,
        cairo.LineCap,
        cairo.LineJoin,
        cairo.Operator,
        cairo.PathDataType,
        cairo.RegionOverlap,
        cairo.SubpixelOrder,
    ]

    if hasattr(cairo, "ColorMode"):
        # cairo 1.17.8+
        types_.append(cairo.ColorMode)

    if hasattr(cairo, "Dither"):
        # cairo 1.18.0+
        types_.append(cairo.Dither)

    def get_prefix(t: type) -> str:
        name = t.__name__
        # special case..
        if name == "PathDataType":
            name = "Path"
        return "_".join([s.upper() for s in re.findall('[A-Z][^A-Z]*', name)])

    for t in types_:
        for name in dir(t):
            if name.upper() != name:
                continue
            value = getattr(t, name)
            assert isinstance(value, t)
            prefix = get_prefix(t)
            assert getattr(cairo, f"{prefix}_{name}") == value

    for name in dir(cairo):
        for t in types_:
            prefix = get_prefix(t)
            if name.startswith(f"{prefix}_"):
                postfix = name[len(prefix) + 1:]
                value = getattr(cairo, name)
                assert getattr(t, postfix) == value

    # some enums are different, check manually
    assert cairo.SVG_VERSION_1_1 == cairo.SVGVersion.VERSION_1_1
    assert isinstance(cairo.SVG_VERSION_1_1, cairo.SVGVersion)
    assert isinstance(cairo.SVGVersion.VERSION_1_1, cairo.SVGVersion)
    assert cairo.SVG_VERSION_1_2 == cairo.SVGVersion.VERSION_1_2
    assert isinstance(cairo.SVG_VERSION_1_2, cairo.SVGVersion)
    assert isinstance(cairo.SVGVersion.VERSION_1_2, cairo.SVGVersion)

    assert cairo.PDF_VERSION_1_4 == cairo.PDFVersion.VERSION_1_4
    assert isinstance(cairo.PDF_VERSION_1_4, cairo.PDFVersion)
    assert isinstance(cairo.PDFVersion.VERSION_1_4, cairo.PDFVersion)
    assert cairo.PDF_VERSION_1_5 == cairo.PDFVersion.VERSION_1_5
    assert isinstance(cairo.PDF_VERSION_1_5, cairo.PDFVersion)
    assert isinstance(cairo.PDFVersion.VERSION_1_5, cairo.PDFVersion)

    assert cairo.PS_LEVEL_2 == cairo.PSLevel.LEVEL_2
    assert isinstance(cairo.PS_LEVEL_2, cairo.PSLevel)
    assert isinstance(cairo.PSLevel.LEVEL_2, cairo.PSLevel)
    assert cairo.PS_LEVEL_3 == cairo.PSLevel.LEVEL_3
    assert isinstance(cairo.PS_LEVEL_3, cairo.PSLevel)
    assert isinstance(cairo.PSLevel.LEVEL_3, cairo.PSLevel)


def test_pickle() -> None:
    # These constants used to be plain int. Try to pickle to int so that
    # there is no dependency on pycairo when unpickling.

    value = cairo.Antialias(42)
    new_value = pickle.loads(pickle.dumps(value))
    assert type(new_value) is int
