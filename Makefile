lib = -lumfpack -lopenblas -pthread\
      -lamd  -lcholmod -lcxsparse  \
      -lsuitesparseconfig -lcolamd \
      -lcamd -lccolamd -lmetis -lrt
EXE = FRW-BCM
DEFINES = -g -ggdb -Wall -Wno-unused-result
MYINCDIR = ./include
INCDIR =  -I$(MYINCDIR)
LIBS = -L/home/ming/lib/zhangbl/lib -lopenblas
CC = g++ -std=c++11
PATH_INCLUDE=-I/home/ming/lib/zhangbl/include
OPTFLAG= -O0 
CFLAGS = $(OPTFLAG) $(DEFINES) $(INCDIR)
COMPILE = $(CC) $(CFLAGS) -c

SUB= main.o \
	     countblock.o global.o\
	     integration.o \
             Matrix_Sparse.o \
             FDMSolver.o \
             Configure.o \
             Debugger.o \
             frwbcm.o
             
YACC=obj/y.tab.o

SUBJECTS=$(SUB:%.o=obj/%.o)

ALL: bin/$(EXE)

bin/$(EXE): $(SUBJECTS) $(YACC)
	$(CC) $(CFLAGS) -o  bin/$(EXE) $(SUBJECTS) $(YACC) $(LIBS) $(lib)
 
obj/y.tab.o: src/yf2.y
	yacc -o src/y.tab.cpp src/yf2.y 
	$(COMPILE) -o obj/y.tab.o src/y.tab.cpp $(MYINCLDIR) 
	
obj/%.o: src/%.cpp
	$(COMPILE) -o  $@ $(PATH_INCLUDE) $< $(MYINCLDIR)

clean:
	\rm -f ./obj/*.o ./src/y.tab.cpp bin/FRW-BCM
