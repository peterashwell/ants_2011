CC=g++
CFLAGS=-O3 -funroll-loops
LDFLAGS=-O2 -lm

all:
	g++ -g -O2 -funroll-loops -lm GeneticBot.cc Genome.cc utils/LocalData.cc utils/utils.cc utils/AntManager.cc utils/State.cc genes/Gene.cc genes/ExploreGene.cc run.cc -o MyBot
clean:
	rm -rf *.o
