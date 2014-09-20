from distutils.core import setup, Extension
import platform
system=platform.system()

extra_compile_args=[]

if system=='Linux':
    extra_compile_args=['-std=c++11']

if system=='Windows':
    extra_compile_args=['/O2']

setup(name='exactcover_cxx',
      version='1.0',
      description='Exact Cover / Dancing Links solver',
      author='Dave Compton',
      author_email='davecompton7@gmail.com',
      py_modules=['exactcover_cxx'],
      ext_modules=[
          Extension('_exactcover_cxx', 
              ['Answer.cxx', 'Coverings.cxx', 'exactcover_cxx.i'], 
              extra_compile_args=extra_compile_args, 
              swig_opts=['-threads', '-c++'])],
     )

