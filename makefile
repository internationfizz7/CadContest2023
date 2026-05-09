all: goto

TARGET = NCKUFplanner # Program Name

DIR_INC = ./include
DIR_SRC = ./src

CPP = g++
CPPFLAG = -O3 -g -I $(DIR_INC)

SRCS:= ${wildcard $(DIR_SRC)/*.cpp $(DIR_SRC)/*.c $(DIR_SRC)/*.cc}
OBJS:= ${patsubst %.c, %.o, $(SRCS)}
OBJS:= ${patsubst %.cpp, %.o, $(OBJS)}
OBJS:= ${patsubst %.cc, %.o, $(OBJS)}


# Library
PACKAGE_ROOT = ./Package

#DEFLEF
DEFLEF_INC = -I $(PACKAGE_ROOT)/DEFLEF/lef/include/ -I $(PACKAGE_ROOT)/DEFLEF/def/include/
DEFLEF_LIB = -L $(PACKAGE_ROOT)/DEFLEF/lef/lib/ -llef -L $(PACKAGE_ROOT)/DEFLEF/def/lib/ -ldef

#FLUTE
FLUTE_INC = -I $(PACKAGE_ROOT)/flute_for_HIMAX/
FLUTE_LIB = -L $(PACKAGE_ROOT)/flute_for_HIMAX/ -lflute

#LASPACK
LASPACK_ROOT = $(PACKAGE_ROOT)/laspack
ILASPACK = -I $(LASPACK_ROOT)/include
LLASPACK = -L $(LASPACK_ROOT)/lib
LASPACK  = $(ILASPACK) $(LLASPACK)

#LEMON 1.2.4
LEMON_INC = -I $(PACKAGE_ROOT)/lemon-1.2.4/build/include/
LEMON_LIB = -L $(PACKAGE_ROOT)/lemon-1.2.4/build/lib/ -lemon


goto: $(SRCS) $(OBJS)
	$(CPP) $(CPPFLAG) $(OBJS) -o $(TARGET) -no-pie -lpthread -lm $(DEFLEF_LIB) $(FLUTE_LIB) $(LEMON_LIB) $(LLASPACK) -llaspack -lxc

%.o : %.c
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ILASPACK) $(DEFLEF_INC) $(FLUTE_INC) $(LEMON_INC)
%.o: %.cpp
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ILASPACK) $(DEFLEF_INC) $(FLUTE_INC) $(LEMON_INC)
%.o: %.cc
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ILASPACK) $(DEFLEF_INC) $(FLUTE_INC) $(LEMON_INC)


clean:
	rm -f $(TARGET) $(DIR_SRC)/*.o *.o core.*
