BIN = penta
COMPILER_OPTIONS=-W -Wall -Wextra -pedantic -std=c++11 "-I$(DEVL)/src"

SRCS = \
    penta.cpp 

default : $(BIN)
include $(DEVL)/src/makeConfig/makeConfig
