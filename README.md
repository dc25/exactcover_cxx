# Extension Module: exactcover_cxx 
## Overview

This project contains a c++ implementation of Donald Knuth's dancing links 
algorithm and a swig based python interface to allow access to it using python.

The python interface is written to be easily used by David Goodger's Polyform
Puzzler project: http://sourceforge.net/projects/puzzler/

Integrating this module into Polyform Puzzler results in faster run times.

## Technical Details

The code makes use of language features that require C++11 ( or later ).

The code builds and has been tested on Windows (windows7) and Linux (Ubuntu 
13.04 )

I used Visual Studio 2013 Express Edition on windows and gcc 4.7.3 on Linux.
### Building on Linux
To build on Linux you will need 
* the python development environment - I used python 2.7.5
* g++ - I used gcc 4.7.3
* swig - I used swig 2.0.8


To build/install the extension on Linux bring up an interactive shell.
Change your working directory to the src directory.

Now type:

```
python setup.py build_ext
python setup.py install
```

The first command will build the extension.  You will see a few warnings but no errors.  The second command will add the extension to your python environment.

### Building on Windows
To build on Windows you will need 
* The python development environment - I used python 2.7.8
* A version of Microsoft Visual Studio that supports c++11 - I used Visual Studio 2013 Express Edition (available for free download).
* swig - I used swig 2.0.12 .


To build/install the extension on Linux bring up an interactive shell.
Change your working directory to the src directory.

Now type:

python setup.py build_ext
python setup.py install

The first command will build the extension.  You will see a few warnings but no errors.  The second command will add the extension to your python environment.

To build/install the extension on windows, first bring up a "Developer 
Command Prompt for VS2012". This can be found in your start menu if you 
have installed Visual Studio 2012 Express Edition.  You will need to set an 
environment variable for setup.py to work correctly:

```
SET VS90COMNTOOLS=%VS110COMNTOOLS% 
```

*(from this point on the build process is the same on Windows as on Linux)*

Change your working directory to the src directory.

Now type:

python setup.py build_ext
python setup.py install

The first command will build the extension.  You will see a few warnings but no errors.  The second command will add the extension to your python environment.


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
