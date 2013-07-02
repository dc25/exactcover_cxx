default: lib

LIBNAME = exactcover

SRCS = \
	exactcover_wrap.cxx\
	Coverings.cxx 

OBJS =  $(SRCS:.cxx=.o)
LIB = _$(LIBNAME).so

COMPILER_OPTIONS=-W -Wall -Wextra -pedantic -std=c++11 
LDFLAGS=  -fPIC -shared -o 
INCLUDE_DIRS=/usr/include/python2.7

DEFINE_FLAGS=$(foreach s, $(DEFINES), -D$(s))
INCLUDE_FLAGS=$(foreach s, $(INCLUDE_DIRS), -I$(s))
CXXFLAGS=-g  $(INCLUDE_FLAGS) $(DEFINE_FLAGS) $(COMPILER_OPTIONS)

objs: $(OBJS)
lib : $(LIB)

$(LIB) : $(OBJS)
	$(CXX) $(LDFLAGS) $(LIB) $(OBJS) $(LLDLIBS) 

%.o : %.cxx
	g++ -c $(CXXFLAGS) $< -o $@


