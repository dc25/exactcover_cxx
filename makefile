# Dancing Links / Exact Cover library with c++ and python interfaces
# Copyright (c) 2013 by David Compton
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

default: lib

LIBNAME = exactcover_cxx

SRCS = \
	exactcover_cxx_wrap.cxx\
	Coverings.cxx 

OBJS =  $(SRCS:.cxx=.o)
LIB = _$(LIBNAME).so

COMPILER_OPTIONS=-W -Wall -Wextra -pedantic -std=c++11 
LDFLAGS=  -fPIC -shared -o 
INCLUDE_DIRS=/usr/include/python2.7

DEFINE_FLAGS=$(foreach s, $(DEFINES), -D$(s))
INCLUDE_FLAGS=$(foreach s, $(INCLUDE_DIRS), -I$(s))
CXXFLAGS=-g  $(INCLUDE_FLAGS) $(DEFINE_FLAGS) $(COMPILER_OPTIONS)

objs: run_swig $(OBJS)
lib : $(LIB)

$(LIB) : objs
	$(CXX) $(LDFLAGS) $(LIB) $(OBJS) $(LLDLIBS) 

%.o : %.cxx
	g++ -c $(CXXFLAGS) $< -o $@

run_swig : 
	swig -c++ -python exactcover_cxx.i


