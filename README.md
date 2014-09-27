# Polyform Puzzler Extension Module: exactcover_cxx 

This project contains a c++ implementation of Donald Knuth's dancing links 
algorithm and a swig based python interface to allow access to it using python.

The python interface is written to be easily used by David Goodger's polyform
puzzler project: http://sourceforge.net/projects/puzzler/

The code builds and has been tested on Windows (windows7) and Linux (Ubuntu 
13.04 )

The c++ code makes use of language features that require C++11 ( or later ).

I used Visual Studio 2012 Express Edition on windows and gcc 4.7.3 on Linux.

To build/install the python extension, "exactcover_cxx", you will also need 
the python 2.7 development environment ( I used 2.7.5 ) and swig ( I used 
swig version 2.0.8 ).

To build/install the extension on Linux bring up an interactive shell.
Change your working directory to the directory containing setup.py 
(the same directory as this file).  

Now type:

python setup.py build_ext
python setup.py install

The first command will build the extension.  You will see a few warnings but no errors.  The second command will install the extension.

To build/install the extension on windows, first bring up a "Developer 
Command Prompt for VS2012". This can be found in your start menu if you 
have installed Visual Studio 2012 Express Edition.  You will need to set an 
environment variable for setup.py to work correctly:

SET VS90COMNTOOLS=%VS110COMNTOOLS% 

Once this is done, the build process should be the same on windows as on linux.

If you prefer, VS2012 Express Edition solution/project files are available for
windows and a makefile is available that should work on Linux.

The windows solution/project files are : exactcover_cxx.sln and 
exactcover_cxx.vcxproj .   If you go that route, you may need to 
specify the directories where the Python include and library files 
are to be found.  The prebuilt Python distributions do not come with
a debug version of the python library so you will probably need to specify 
the "Solution Configuration", "Release" from within Visual Studio.

Once you have built using any of the above methods you should be able to test
your build by running "python pentominoes.py" from the command line. You
should see the following output.

Example covering:

llxuuvvv
lxxxuvzz
lfxuuvzn
lff  zzn
ffy  wnn
yyyywwnt
pppwwttt
ppiiiiit

I copied the pentominos.py script from https://github.com/kwaters/exactcover
which is a similar project to this one.
