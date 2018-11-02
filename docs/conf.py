# -*- coding: utf-8 -*-

extensions = [
    'sphinx.ext.intersphinx',
    'sphinx.ext.extlinks',
]
intersphinx_mapping = {
    'python': ('https://docs.python.org/2', None),
    'python3': ('https://docs.python.org/3', None),
}
source_suffix = '.rst'
master_doc = 'index'
project = u'Pycairo'
html_show_copyright = False
exclude_patterns = ['_build']
pygments_style = 'sphinx'
html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "display_version": False,
}
html_context = {
    'extra_css_files': [
        '_static/extra.css',
    ],
}

html_static_path = [
    "extra.css",
]

extlinks = {
    'fdobug': ('https://bugs.freedesktop.org/show_bug.cgi?id=%s', '#fdo-'),
    'bug': ('https://github.com/pygobject/pycairo/issues/%s', '#'),
    'pr': ('https://github.com/pygobject/pycairo/pull/%s', '#pr-'),
    'user': ('https://github.com/%s', ''),
}
suppress_warnings = ["image.nonlocal_uri"]
