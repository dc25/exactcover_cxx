from distutils.core import setup, Extension

setup(name='exactcover',
      version='1.0',
      ext_modules=[
          Extension('_exactcover', ['exactcover.i'], swig_opts=['-c++', '-python'])],
      )

