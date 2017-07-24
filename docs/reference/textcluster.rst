.. _textcluster:

************
Text Cluster
************

.. currentmodule:: cairo

class TextCluster(tuple)
========================

.. class:: TextCluster(num_bytes, num_glyphs)

    :param int num_bytes:
        the number of bytes of UTF-8 text covered by cluster
    :param int num_glyphs:
        the number of glyphs covered by cluster

    .. versionadded:: 1.15

    The :class:`TextCluster` structure holds information about a single text
    cluster. A text cluster is a minimal mapping of some glyphs corresponding
    to some UTF-8 text.

    For a cluster to be valid, both ``num_bytes`` and ``num_glyphs`` should be
    non-negative, and at least one should be non-zero. Note that clusters with
    zero glyphs are not as well supported as normal clusters. For example, PDF
    rendering applications typically ignore those clusters when PDF text is
    being selected.

    See :meth:`Context.show_text_glyphs` for how clusters are used in advanced
    text operations.

    .. attribute:: num_bytes

        :class:`int`

    .. attribute:: num_glyphs

        :class:`int`
