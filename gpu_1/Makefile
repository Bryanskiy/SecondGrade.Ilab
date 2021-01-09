.PHONY: all local_preproc global_preproc

LOCAL_OPT     = -D "LOCAL_PREPROC_"
GLOBAL_OPT    = -D "GLOBAL_PREPROC_"
ITERATIVE_OPT = -D "ITERATIVE_"

all:
	g++ main.cpp bsort.cpp -lOpenCL -o main.out $(ITERATIVE_OPT)
	g++ ./tests/random_test.cpp bsort.cpp -lOpenCL -o random_test.out $(ITERATIVE_OPT)

local_preproc:
	g++ main.cpp bsort.cpp -lOpenCL -o main.out $(LOCAL_OPT)
	g++ ./tests/random_test.cpp bsort.cpp -lOpenCL -o random_test.out $(LOCAL_OPT)

global_preproc:
	g++ main.cpp bsort.cpp -lOpenCL -o main.out $(GLOBAL_OPT)
	g++ ./tests/random_test.cpp bsort.cpp -lOpenCL -o random_test.out $(GLOBAL_OPT)