from distutils.core import setup, Extension

setup(name='exactcover',
      version='1.0',
      ext_modules=[
          Extension('_exactcover', ['Coverings.cxx', 'exactcover.i'], extra_compile_args=['-std=c++11'], swig_opts=['-c++'])],
      )

