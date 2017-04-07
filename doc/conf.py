# -*- coding: utf-8 -*-

import sys, os

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
version = '1.10'
release = '1.10.1'
exclude_patterns = ['_build']
pygments_style = 'sphinx'
html_theme = "sphinx_rtd_theme"
