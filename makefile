
#--------------------------------------------------------------------------

CC=g++
CFLAGS=-Wall -Iinclude -fopenmp
TESTFLAGS=

INPUT_FILE=./datasets/nasd_input.csv
QUERY_FILE=./datasets/nasd_query.csv
CONFIG_FILE=./cluster.conf

SEARCH_OUTPUT_FILE=search_out.txt

SRCS=$(wildcard ./src/*.cpp)
OBJS=$(patsubst ./src/%.cpp, ./obj/%.o, $(SRCS))

OUT_SEARCH=./bin/search

RUNFLAGS_SEARCH_LSH= -algorithm LSH -delta 2 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_CUB= -algorithm Hypercube -delta 2 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_DIS= -algorithm Frechet -metric discrete -delta 2 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_SEARCH_CON= -algorithm Frechet -metric continuous -delta 2 -o $(SEARCH_OUTPUT_FILE) -i $(INPUT_FILE) -q $(QUERY_FILE)

VGFLAGS=--leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes

#--------------------------------------------------------------------------

all: $(OUT_SEARCH)

$(OUT_SEARCH): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) /usr/lib/x86_64-linux-gnu/libcppunit.a -o $@

./obj/%.o: ./src/%.cpp
	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

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

test: $(OUT_SEARCH)
	$(OUT_SEARCH) $(RUNFLAGS_SEARCH_CON)

#--------------------------------------------------------------------------

VG_RUNFLAGS=$(RUNFLAGS_SEARCH_DIS)

vg_search: $(OUT_SEARCH)
	valgrind $(OUT_SEARCH) $(VG_RUNFLAGS)

vgext_search: $(OUT_SEARCH)
	valgrind $(VGFLAGS) $(OUT_SEARCH) $(VG_RUNFLAGS)

#--------------------------------------------------------------------------

run: lsh

vg: vg_search

vgext: vgext_search

.PHONY: clean all run lsh cube disc cont vg_search vgext_search vg vgext

#--------------------------------------------------------------------------
