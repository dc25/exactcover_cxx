from distutils.core import setup, Extension

setup(name='exactcover_cxx',
      version='1.0',
      py_modules=['exactcover_cxx'],
      ext_modules=[
          Extension('_exactcover_cxx', 
              ['Coverings.cxx', 'exactcover_cxx.i'], 
              extra_compile_args=['-std=c++11'], 
              swig_opts=['-c++'])],
     )

