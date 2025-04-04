import os
import sys


dir_ = os.path.dirname(os.path.realpath(__file__))


def exec_module(path):
    import importlib.machinery
    loader = importlib.machinery.SourceFileLoader("cairo", path)
    return loader.load_module()


sys.modules["cairo"] = exec_module(os.path.join(dir_, "..", "cairo", "__init__.pyi"))

extensions = [
    'sphinx.ext.intersphinx',
    'sphinx.ext.extlinks',
    'sphinx.ext.autodoc',
]
intersphinx_mapping = {
    'python3': ('https://docs.python.org/3', None),
}
source_suffix = '.rst'
master_doc = 'index'
project = 'Pycairo'
html_show_copyright = False
exclude_patterns = ['_build']
pygments_style = 'friendly'
html_theme = "sphinx_rtd_theme"
html_theme_options = {
    'version_selector': False,
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
    'fdobug': ('https://bugs.freedesktop.org/show_bug.cgi?id=%s', '#fdo-%s'),
    'bug': ('https://github.com/pygobject/pycairo/issues/%s', '#%s'),
    'pr': ('https://github.com/pygobject/pycairo/pull/%s', '#pr-%s'),
    'user': ('https://github.com/%s', '%s'),
}
suppress_warnings = ["image.nonlocal_uri"]

autoclass_content = 'class'

autodoc_default_options = {
    'member-order': 'bysource',
}

autodoc_type_aliases = {
    '_PathLike': 'cairo._PathLike',
    '_FileLike': 'cairo._FileLike',
}
