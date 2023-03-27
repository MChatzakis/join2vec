#### Basis
CC = gcc
CXX = c++
CXXFLAGS = -pthread -std=c++11 -O3 -march=native
EXECUTABLES_PATH = ../bin/
EVALUATION_PATH = ../evaluation/


#### General
COMMON_OBJS = timers.o utilfuncs.o concurrency.o
COMMON_PATH = ../src/common/


####SIMD
SIMD_LINK_DIR = ../src/Simd/build/


#### FastText Compilation Dependencies
FT_OBJS = args.o autotune.o matrix.o dictionary.o loss.o productquantizer.o densematrix.o quantmatrix.o vector.o model.o utils.o meter.o fasttext.o fasttext_lib.o
FT_PATH = ../src/j2v/fastText/
FT_LIB_PATH = ../src/j2v/


#### j2v Compilation Dependencies
J2V_PATH = ../src/j2v/
J2V_OBJS = $(FT_OBJS) join2vec.o


#### classic-simjoin
CSJ_PATH = ../src/classic-simjoin/
CSJ_COMMONS = $(CSJ_PATH)classic-simjoin-common/*.h
CSJ_FLAGS = -std=c++11 -Wall -Wno-unused-result -O3 -MMD -MP -DNDEBUG -pthread


#### Scripts
SCRIPTS_PATH = ../scripts/


#### Create Everything
all: j2v wiki-ds-creator fastss


#### General Part
utilfuncs.o: $(COMMON_PATH)utilfuncs.cpp $(COMMON_PATH)utilfuncs.h 
	$(CXX) $(CXXFLAGS) -c $(COMMON_PATH)utilfuncs.cpp

timers.o: $(COMMON_PATH)timers.cpp $(COMMON_PATH)timers.h 
	$(CXX) $(CXXFLAGS) -c $(COMMON_PATH)timers.cpp

concurrency.o: $(COMMON_PATH)concurrency.cpp $(COMMON_PATH)concurrency.h 
	$(CXX) $(CXXFLAGS) -c $(COMMON_PATH)concurrency.cpp


#### FastText and J2V Part
args.o:	$(FT_PATH)src/args.cc $(FT_PATH)src/args.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/args.cc

autotune.o:	$(FT_PATH)src/autotune.cc  $(FT_PATH)src/autotune.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/autotune.cc

matrix.o:  $(FT_PATH)src/matrix.cc  $(FT_PATH)src/matrix.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/matrix.cc

dictionary.o:  $(FT_PATH)src/dictionary.cc  $(FT_PATH)src/dictionary.h $(FT_PATH)src/args.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/dictionary.cc

loss.o:  $(FT_PATH)src/loss.cc  $(FT_PATH)src/loss.h $(FT_PATH)src/matrix.h $(FT_PATH)src/real.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/loss.cc

productquantizer.o:  $(FT_PATH)src/productquantizer.cc $(FT_PATH)src/productquantizer.h $(FT_PATH)src/utils.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/productquantizer.cc

densematrix.o:  $(FT_PATH)src/densematrix.cc $(FT_PATH)src/densematrix.h $(FT_PATH)src/utils.h $(FT_PATH)src/matrix.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/densematrix.cc

quantmatrix.o:  $(FT_PATH)src/quantmatrix.cc $(FT_PATH)src/quantmatrix.h  $(FT_PATH)src/utils.h $(FT_PATH)src/matrix.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/quantmatrix.cc

vector.o:  $(FT_PATH)src/vector.cc  $(FT_PATH)src/vector.h $(FT_PATH)src/utils.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/vector.cc

model.o:  $(FT_PATH)src/model.cc $(FT_PATH)src/model.h $(FT_PATH)src/args.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/model.cc

utils.o:  $(FT_PATH)src/utils.cc $(FT_PATH)src/utils.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/utils.cc

meter.o:  $(FT_PATH)src/meter.cc $(FT_PATH)src/meter.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/meter.cc

fasttext.o:  $(FT_PATH)src/fasttext.cc $(FT_PATH)src/*.h
	$(CXX) $(CXXFLAGS) -c $(FT_PATH)src/fasttext.cc

fasttext_lib.o: $(FT_LIB_PATH)fasttext_lib.cpp $(FT_LIB_PATH)fasttext_lib.h 
	$(CXX) $(CXXFLAGS) -c $(FT_LIB_PATH)fasttext_lib.cpp 

join2vec.o: $(J2V_PATH)join2vec.cpp $(J2V_PATH)join2vec.h 
	$(CXX) $(CXXFLAGS) -c $(FT_LIB_PATH)join2vec.cpp 

j2v: $(COMMON_OBJS) $(J2V_OBJS) $(J2V_PATH)main.cpp 
	$(CXX) $(CXXFLAGS) $(COMMON_OBJS) $(J2V_OBJS) -L$(SIMD_LINK_DIR) $(J2V_PATH)main.cpp -lSimd -o $(EXECUTABLES_PATH)j2v


#### BruteForce Part
bf: $(COMMON_OBJS) $(CSJ_PATH)bruteforce/bruteforce.cpp
	$(CXX) $(CXXFLAGS) $(COMMON_OBJS) -L$(SIMD_LINK_DIR) $(CSJ_PATH)bruteforce/bruteforce.cpp -lSimd -o $(EXECUTABLES_PATH)bf


#### FastSS
fastss: $(CSJ_COMMONS) $(COMMON_OBJS) $(CSJ_PATH)fastss-ed/fastss.cpp
	$(CXX) $(CSJ_FLAGS) $(COMMON_OBJS) -L$(SIMD_LINK_DIR) $(CSJ_PATH)fastss-ed/fastss.cpp -lSimd -o $(EXECUTABLES_PATH)fastss
	rm $(EXECUTABLES_PATH)fastss.d


#### AdaptJoin
adaptjoin: $(CSJ_COMMONS) $(CSJ_PATH)adaptjoin/*
	$(CXX) $(CSJ_FLAGS) -L$(SIMD_LINK_DIR) $(CSJ_PATH)adaptjoin/*.cpp -lSimd -o $(EXECUTABLES_PATH)adaptjoin
	rm $(EXECUTABLES_PATH)adaptjoin.d


#### ppJoin+
ppjoinplus: $(CSJ_COMMONS) $(CSJ_PATH)ppjoin/ppjoin.cpp
	$(CXX) $(CSJ_FLAGS) -L$(SIMD_LINK_DIR) $(CSJ_PATH)ppjoin/ppjoin.cpp -lSimd -o $(EXECUTABLES_PATH)ppjoin-plus -DPPJOIN -DPLUS
	rm $(EXECUTABLES_PATH)ppjoin-plus.d

#### Dataset Scripts
wiki-ds-creator: $(SCRIPTS_PATH)wiki-ds-creator.c
	$(CC) $(SCRIPTS_PATH)wiki-ds-creator.c -o $(EXECUTABLES_PATH)wiki-ds-creator  


#### Clean everything :)
clean:
	rm -rf *.o *.gcno *.gcda fasttext *.bc webassembly/fasttext_wasm.js webassembly/fasttext_wasm.wasm


clean-exec:
	rm $(EXECUTABLES_PATH)*