
#--------------------------------------------------------------------------

CC=g++
CFLAGS=-Iinclude -O3 #-fopenmp
TESTLIB=/usr/lib/x86_64-linux-gnu/libcppunit.a

INPUT_FILE=./datasets/nasd_input.csv
QUERY_FILE=./datasets/nasd_query.csv
CONFIG_FILE=./cluster.conf

SEARCH_OUTPUT_FILE=search_out.txt
CLUSTER_OUTPUT_FILE=cluster_out.txt

SRCS=$(wildcard ./src/*.cpp)
OBJS=$(patsubst ./src/%.cpp, ./obj/%.o, $(SRCS))

OBJS_SRC=$(filter-out ./obj/main_cluster.o ./obj/main_test.o ./obj/DistanceUnitTest.o, $(OBJS))
OBJS_CLS=$(filter-out ./obj/main_search.o ./obj/main_test.o ./obj/DistanceUnitTest.o, $(OBJS))
OBJS_TST=$(filter-out ./obj/main_search.o ./obj/main_cluster.o, $(OBJS))

OUT_SEARCH=./bin/search
OUT_CLUSTR=./bin/cluster
OUT_TEST=./bin/test

COMPLETE_FLAG= #-complete
SILHOUETTE_FLAG= -silhouette

RUNFLAGS_SEARCH_LSH= -algorithm LSH -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_CUB= -algorithm Hypercube -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_DIS= -algorithm Frechet -metric discrete -delta 1.618 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_CON= -algorithm Frechet -metric continuous -delta 1.618 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)

RUNFLAGS_CLUSTER_CLA_VEC= -update Mean Vector  -assignment Classic   -i $(INPUT_FILE) -c $(CONFIG_FILE) -o $(CLUSTER_OUTPUT_FILE) $(COMPLETE_FLAG) $(SILHOUETTE_FLAG)
RUNFLAGS_CLUSTER_CLA_FRE= -update Mean Frechet -assignment Classic   -i $(INPUT_FILE) -c $(CONFIG_FILE) -o $(CLUSTER_OUTPUT_FILE) $(COMPLETE_FLAG) $(SILHOUETTE_FLAG)
RUNFLAGS_CLUSTER_LSH_FRE= -update Mean Frechet -assignment LSH       -i $(INPUT_FILE) -c $(CONFIG_FILE) -o $(CLUSTER_OUTPUT_FILE) $(COMPLETE_FLAG) $(SILHOUETTE_FLAG)
RUNFLAGS_CLUSTER_LSH_VEC= -update Mean Vector  -assignment LSH       -i $(INPUT_FILE) -c $(CONFIG_FILE) -o $(CLUSTER_OUTPUT_FILE) $(COMPLETE_FLAG) $(SILHOUETTE_FLAG)
RUNFLAGS_CLUSTER_HYPCUBE= -update Mean Vector  -assignment Hypercube -i $(INPUT_FILE) -c $(CONFIG_FILE) -o $(CLUSTER_OUTPUT_FILE) $(COMPLETE_FLAG) $(SILHOUETTE_FLAG)

VGFLAGS=--leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes

#--------------------------------------------------------------------------

all: $(OUT_SEARCH) $(OUT_CLUSTR)

$(OUT_SEARCH): $(OBJS_SRC)
	$(CC) $(CFLAGS) $(OBJS_SRC) -o $@

$(OUT_CLUSTR): $(OBJS_CLS)
	$(CC) $(CFLAGS) $(OBJS_CLS) -o $@

$(OUT_TEST): $(OBJS_TST)
	$(CC) $(CFLAGS) $(OBJS_TST) -o $@ $(TESTLIB)

./obj/%.o: ./src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r ./bin/* ./obj/*

init:
	mkdir obj bin

#--------------------------------------------------------------------------

lsh: $(OUT_SEARCH)
	$(OUT_SEARCH) $(RUNFLAGS_SEARCH_LSH)

cube: $(OUT_SEARCH)
	$(OUT_SEARCH) $(RUNFLAGS_SEARCH_CUB)

disc: $(OUT_SEARCH)
	$(OUT_SEARCH) $(RUNFLAGS_SEARCH_DIS)

cont: $(OUT_SEARCH)
	$(OUT_SEARCH) $(RUNFLAGS_SEARCH_CON)

#--------------------------------------------------------------------------

clavec: $(OUT_CLUSTR)
	$(OUT_CLUSTR) $(RUNFLAGS_CLUSTER_CLA_VEC)

clafre: $(OUT_CLUSTR)
	$(OUT_CLUSTR) $(RUNFLAGS_CLUSTER_CLA_FRE)

lshfre: $(OUT_CLUSTR)
	$(OUT_CLUSTR) $(RUNFLAGS_CLUSTER_LSH_FRE)

lshvec: $(OUT_CLUSTR)
	$(OUT_CLUSTR) $(RUNFLAGS_CLUSTER_LSH_VEC)

cubvec: $(OUT_CLUSTR)
	$(OUT_CLUSTR) $(RUNFLAGS_CLUSTER_HYPCUBE)

#--------------------------------------------------------------------------

test: $(OUT_TEST)
	$(OUT_TEST)

#--------------------------------------------------------------------------

VG_EXEC=$(OUT_CLUSTR)
VG_RUNFLAGS=$(RUNFLAGS_CLUSTER_CLA_VEC)

vg: $(VG_EXEC)
	valgrind $(VG_EXEC) $(VG_RUNFLAGS)

vgext: $(VG_EXEC)
	valgrind $(VGFLAGS) $(VG_EXEC) $(VG_RUNFLAGS)

#--------------------------------------------------------------------------

run: lshfre

.PHONY: clean all run lsh cube disc cont clavec clafre lshfre lshvec cubvec vg vgext

#--------------------------------------------------------------------------
