all: goto

TARGET = NCKUFplanner # Program Name

DIR_INC = ./include
DIR_SRC = ./src

CPP = g++ # Which compiler: CPP=g++462
CPPFLAG = -O3 -g -I $(DIR_INC) # Options for development, -g:GDB

SRCS:= ${wildcard $(DIR_SRC)/*.cpp $(DIR_SRC)/*.c $(DIR_SRC)/*.cc}      # wildcard function: find all list file name is this directory
OBJS:= ${patsubst %.c, %.o, $(SRCS)}   # patsubst function: replace name *.c -> *.o (":=" single expend variable, "=" recursively expend variable)
OBJS:= ${patsubst %.cpp, %.o, $(OBJS)} #
OBJS:= ${patsubst %.cc, %.o, $(OBJS)}  #




# Library
PACKAGE_ROOT = ../Integrate/Package

#DEFLEF
DEFLEF_INC = -I ../Integrate/Package/DEFLEF/lef/include/ -I ../Integrate/Package/DEFLEF/def/include/
DEFLEF_LIB = -L ../Integrate/Package/DEFLEF/lef/lib/ -llef -L ../Integrate/Package/DEFLEF/def/lib/ -ldef
#FLUTE
FLUTE_INC = -I ../Integrate/Package/flute_for_HIMAX/
FLUTE_LIB = -L ../Integrate/Package/flute_for_HIMAX/ -lflute
# CPLEX
CPLEX_ROOT = $(PACKAGE_ROOT)/CPLEX_Studio125
ICPLEX   = -I$(CPLEX_ROOT)/cplex/include
ICONCERT = -I$(CPLEX_ROOT)/concert/include
LCPLEX   = -L$(CPLEX_ROOT)/cplex/lib/x86-64_sles10_4.1/static_pic
LCONCERT = -L$(CPLEX_ROOT)/concert/lib/x86-64_sles10_4.1/static_pic
CPLEX    = $(ICPLEX) $(ICONCERT) $(LCPLEX) $(LCONCERT)
# LASPACK
LASPACK_ROOT=$(PACKAGE_ROOT)/laspack
ILASPACK  = -I$(LASPACK_ROOT)/include
LLASPACK  = -L$(LASPACK_ROOT)/lib
LASPACK   = $(ILASPACK) $(LLASPACK)
#LEDA
LEDA_ROOT=$(PACKAGE_ROOT)/leda/leda-current
ILEDA = -I$(LEDA_ROOT)/incl
LLEDA = -L$(LEDA_ROOT)
LEDA  = $(ILEDA) $(LLEDA)
#LEMON
LEMON_ROOT=$(PACKAGE_ROOT)/lemon
ILEMON = -I$(LEMON_ROOT)/include
LLEMON = -L$(LEMON_ROOT)/lib
LEMON  = $(ILEMON) $(LLEMON)
#LEMON1.2.4
LEMON_INC = -I ../Integrate/Package/lemon-1.2.4/build/include/
LEMON_LIB = -L ../Integrate/Package/lemon-1.2.4/build/lib/ -lemon
#FFTW
FFTW_ROOT= $(PACKAGE_ROOT)/FFTW
IFFTW = -I $(FFTW_ROOT)/include
LFFTW = -L $(FFTW_ROOT)/lib
FFTW = $(IFFTW) $(LFFTW)


goto:	$(SRCS) $(OBJS)
	$(CPP) $(CPPFLAG) $(OBJS) -o $(TARGET) $(CPLEX) -DIL_STD -lconcert -lilocplex -lcplex -lpthread -lm $(LASPACK) $(DEFLEF_LIB) $(FLUTE_LIB) ${LEMON_LIB} -llaspack $(LEMON) -lemon $(FFTW) -lfftw3 
#<TAB> <command> : each command start with a TAB (shell script, /bin/sh)


#this use suffix rule $@ $< and pattern rule %
#$@ is target
#$< is first prerequest
#$? is all prerequest, exclude the file without modify
#$^ is all prerequest
%.o : %.c
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ICPLEX) $(ICONCERT) -DIL_STD $(ILASPACK) $(ILEMON) $(IFFTW) $(DEFLEF_INC) $(FLUTE_INC) ${LEMON_INC}
%.o: %.cpp
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ICPLEX) $(ICONCERT) -DIL_STD $(ILASPACK) $(ILEMON) $(IFFTW) $(DEFLEF_INC) $(FLUTE_INC) ${LEMON_INC}
%.o: %.cc
	$(CPP) $(CPPFLAG) -c $< -o $@ $(ICPLEX) $(ICONCERT) -DIL_STD $(ILASPACK) $(ILEMON) $(IFFTW) $(DEFLEF_INC) $(FLUTE_INC) ${LEMON_INC}



clean:
	rm -f $(TARGET) $(DIR_SRC)/*.o *.o core.*
