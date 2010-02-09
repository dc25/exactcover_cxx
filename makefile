BIN = penta
CXXFLAGS=-g '-Ic:/Boost/include/boost-1_33' "-I$(DEVL)/src"
LDFLAGS=-mwindows 
LLDLIBS = $(DEVL)/src/utilities/libutilities.dll.a '-Lc:/Boost/lib' -lboost_thread-gcc-mt -lstdc++

SRCS = \
    penta.cpp 

OBJS =  $(SRCS:.cpp=.o)

default : $(BIN)

$(BIN) : $(OBJS) 
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LLDLIBS)

