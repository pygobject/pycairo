import os
import inspect
import importlib


class Snippet(object):

    name = ""
    """name of the snippet"""

    code = ""
    """Snippet code as text"""

    def draw_func(cr, width, height):
        """Snippet entry point"""

        raise NotImplementedError


def get_snippets():
    """Returns a name:snippet dict"""

    # list of snippet files
    snip_list = [x[:-3] for x in os.listdir(os.path.dirname(__file__))
                 if not x.startswith('_') and x.endswith('.py')]
    snip_list.sort()

    snippets = {}
    for name in snip_list:
        s = Snippet()
        s.name = name
        mod = importlib.import_module("." + name, __package__)
        s.draw_func = getattr(mod, "draw")
        code = inspect.getsource(mod)
        if isinstance(code, bytes):
            code = code.decode("utf-8")
        s.code = code
        snippets[s.name] = s
    return snippets
