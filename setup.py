from distutils.core import setup, Extension

setup(name='exactcover_cxx',
      version='1.0',
      description='Exact Cover / Dancing Links solver',
      author='Dave Compton',
      author_email='davecompton200@gmail.com',
      py_modules=['exactcover_cxx'],
      ext_modules=[
          Extension('_exactcover_cxx', 
              ['Answer.cxx', 'Coverings.cxx', 'exactcover_cxx.i'], 
              extra_compile_args=['-std=c++11'], 
              swig_opts=['-c++'])],
     )

