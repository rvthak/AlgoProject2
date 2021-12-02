
#--------------------------------------------------------------------------

CC=g++
CFLAGS=-Wall -Iinclude

INPUT_FILE=./datasets/input_small_id
QUERY_FILE=./datasets/query_small_id
CONFIG_FILE=./cluster.conf

SRCS=$(wildcard ./src/*.cpp)
OBJS=$(patsubst ./src/%.cpp, ./obj/%.o, $(SRCS))

OBJS_LSH=$(filter-out ./obj/main_Cube.o ./obj/main_Cluster.o ./obj/Silhouette.o, $(OBJS))
OBJS_CUB=$(filter-out ./obj/main_LSH.o  ./obj/main_Cluster.o ./obj/Silhouette.o, $(OBJS))
OBJS_CLS=$(filter-out ./obj/main_LSH.o  ./obj/main_Cube.o, $(OBJS))

OUT_LSH=./bin/lsh
OUT_CUB=./bin/cube
OUT_CLS=./bin/cluster

RUNFLAGS_LSH=-o lsh_out.txt  -k 5 -L 3 -N 6 -R 450            -i $(INPUT_FILE) -q $(QUERY_FILE)
RUNFLAGS_CUB=-o cube_out.txt -k 10 -M 24 -N 6 -R 450 -probes 3 -i $(INPUT_FILE) -q $(QUERY_FILE)

RUNFLAGS_CLS_CLAS=-o cluster_out_classic.txt -m Classic   -complete -i $(INPUT_FILE) -c $(CONFIG_FILE)
RUNFLAGS_CLS_LSH =-o cluster_out_lsh.txt     -m LSH       -complete -i $(INPUT_FILE) -c $(CONFIG_FILE)
RUNFLAGS_CLS_CUBE=-o cluster_out_cube.txt    -m Hypercube -complete -i $(INPUT_FILE) -c $(CONFIG_FILE)

VGFLAGS=--leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes

#--------------------------------------------------------------------------

all: $(OUT_LSH) $(OUT_CUB) $(OUT_CLS)

$(OUT_LSH): $(OBJS_LSH)
	$(CC) $(CFLAGS) $(OBJS_LSH) -o $@

$(OUT_CUB): $(OBJS_CUB)
	$(CC) $(CFLAGS) $(OBJS_CUB) -o $@

$(OUT_CLS): $(OBJS_CLS)
	$(CC) $(CFLAGS) $(OBJS_CLS) -o $@

./obj/%.o: ./src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r ./bin/* ./obj/*

init:
	mkdir obj bin

#--------------------------------------------------------------------------

lsh: $(OUT_LSH)
	$(OUT_LSH) $(RUNFLAGS_LSH)

cube: $(OUT_CUB)
	$(OUT_CUB) $(RUNFLAGS_CUB)

cluster: $(OUT_CLS)
	$(OUT_CLS) $(RUNFLAGS_CLS_CLAS)

cluster_lsh: $(OUT_CLS)
	$(OUT_CLS) $(RUNFLAGS_CLS_LSH)

cluster_cube: $(OUT_CLS)
	$(OUT_CLS) $(RUNFLAGS_CLS_CUBE)

#--------------------------------------------------------------------------

vg_lsh: $(OUT_LSH)
	valgrind $(OUT_LSH) $(RUNFLAGS_LSH)

vgext_lsh: $(OUT_LSH)
	valgrind $(VGFLAGS) $(OUT_LSH) $(RUNFLAGS_LSH)

vg_cube: $(OUT_CUB)
	valgrind $(OUT_CUB) $(RUNFLAGS_CUB)

vgext_cube: $(OUT_CUB)
	valgrind $(VGFLAGS) $(OUT_CUB) $(RUNFLAGS_CUB)

vg_cluster: $(OUT_CLS)
	valgrind $(OUT_CLS) $(RUNFLAGS_CLS_LSH)

vgext_cluster: $(OUT_CLS)
	valgrind $(VGFLAGS) $(OUT_CLS) $(RUNFLAGS_CLS_LSH)

#--------------------------------------------------------------------------

run: cluster

vg: vg_cluster

vgext: vgext_cluster

.PHONY: clean all run lsh cube cluster cluster_lsh cluster_cube vg vgext vg_lsh vgext_lsh vg_cube vgext_cube vg_cluster vgext_cluster

#--------------------------------------------------------------------------
