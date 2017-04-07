# -*- coding: utf-8 -*-

extensions = [
    'sphinx.ext.intersphinx',
]
intersphinx_mapping = {
    'python': ('https://docs.python.org/2.7', None),
    'python3': ('https://docs.python.org/3.6', None),
}
source_suffix = '.rst'
master_doc = 'index'
project = u'Pycairo'
copyright = u'2008, Steve Chaplin'
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
