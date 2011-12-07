CC=g++
CFLAGS=-funroll-loops
LDFLAGS=-lm

all:
	g++ -g -funroll-loops -lm utils/utils.cc GeneticBot.cc Genome.cc utils/LocalData.cc utils/AntManager.cc utils/State.cc genes/Gene.cc genes/BruteCombatGene.cc genes/HillGene.cc genes/ExploreGene.cc genes/PositionGene.cc genes/FoodGene.cc utils/ClusterCells.cc MyBot.cc -o MyBot
clean:
	rm -rf *.o
	rm -rf *.gch
