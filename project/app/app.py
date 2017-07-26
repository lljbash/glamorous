# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.8
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_app', [dirname(__file__)])
        except ImportError:
            import _app
            return _app
        if fp is not None:
            try:
                _mod = imp.load_module('_app', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _app = swig_import_helper()
    del swig_import_helper
else:
    import _app
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0


class GlamorousApp(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, GlamorousApp, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, GlamorousApp, name)
    __repr__ = _swig_repr

    def initialize(self, db_path):
        return _app.GlamorousApp_initialize(self, db_path)

    def transfer(self, text, style):
        return _app.GlamorousApp_transfer(self, text, style)

    def __init__(self):
        this = _app.new_GlamorousApp()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _app.delete_GlamorousApp
    __del__ = lambda self: None
GlamorousApp_swigregister = _app.GlamorousApp_swigregister
GlamorousApp_swigregister(GlamorousApp)

# This file is compatible with both classic and new-style classes.


