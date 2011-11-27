CC=g++
CFLAGS=-O3 -funroll-loops -c
LDFLAGS=-O2 -lm
SOURCES= GeneticBot.cc utils/State.cc utils/AntManager.cc genes/ExploreGene.cc genes/Gene.cc Genome.cc utils/LocalData.cc utils/utils.cc
OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=run

#Uncomment the following to enable debugging
#CFLAGS+=-g -DDEBUG

g++ -O3 -funroll-loops -c GeneticBot.cc utils/State.cc utils/AntManager.cc genes/ExploreGene.cc genes/Gene.cc Genome.cc utils/LocalData.cc utils/utils.cc
all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o: *.h
	$(CC) $(CFLAGS) $< -o $@

clean: 
	-rm -f ${EXECUTABLE} ${OBJECTS} *.d
	-rm -f debug.txt

.PHONY: all clean

