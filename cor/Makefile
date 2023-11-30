
# Version 2020/05/29
# Author m107jjchen

# this use suffix rule $@ $< and pattern rule %
# $@ is target
# $< is first prerequest
# $? is all prerequest, exclude the file without modify
# $^ is all prerequest
# -----

TARGET 	= ./bin/MacroPlacement
INC_DIR = ./include/ 
SRC_DIR	= ./src_code/

#DEFLEF
DEFLEF_INC = -I ../Package/DEFLEF/lef/include/ -I ../Package/DEFLEF/def/include/
DEFLEF_LIB = -L ../Package/DEFLEF/lef/lib/ -llef -L ../Package/DEFLEF/def/lib/ -ldef


#FLUTE
FLUTE_INC = -I ../Package/flute_for_HIMAX/
FLUTE_LIB = -L ../Package/flute_for_HIMAX/ -lflute

#LASPACK
LASPACK_INC = -I ../Package/laspack/include/
LASPACK_LIB = -L ../Package/laspack/lib/ -lxc -llaspack

#LEMON
LEMON_INC = -I ../Package/lemon-1.2.4/build/include/
LEMON_LIB = -L ../Package/lemon-1.2.4/build/lib/ -lemon

#CPLEX # do not used now

#CPLEX_DIR      = /ift1/PackageDemo/Package_demo/CPLEX_Studio125/cplex
#CONCERT_DIR    = /ift1/PackageDemo/Package_demo/CPLEX_Studio125/concert

#CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
#CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread

#CPLEX_INC = -I $(CONCERT_DIR)/include -I $(CPLEX_DIR)/include $(CCOPT)
#CPLEX_LIB   = -L $(CPLEX_DIR)/lib/x86-64_sles10_4.1/static_pic -L $(CONCERT_DIR)/lib/x86-64_sles10_4.1/static_pic $(CCLNFLAGS)

# -----

CXX= g++
CXXFLAGS= -Wall -g -O3 

SRCS := ${wildcard $(SRC_DIR)*.cpp}
OBJS := ${patsubst %.cpp, %.o, $(SRCS)}
INCS = $(DEFLEF_INC) $(FLUTE_INC) $(LASPACK_INC) ${LEMON_INC} ${CPLEX_INC} -I $(INC_DIR) 
LIBS = $(DEFLEF_LIB) $(FLUTE_LIB) $(LASPACK_LIB) ${LEMON_LIB} ${CPLEX_LIB} -lpthread

# -----

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(SRC_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) ${INCS} -o $@ -c $<

.PHONY: clean
clean:
	rm -rf $(TARGET) $(SRC_DIR)*.o 



