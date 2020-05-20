CC=g++ -std=c++11
CFLAGS=-c -Wpedantic -Ofast -Wall
SCRIPT = create_infiles.sh
SOURCES=main.cpp date.cpp record.cpp ht.cpp aht.cpp bb.cpp tree.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=diseaseAggregator
ARGS= -w 3 -b 64 -i ../folder2
#-w -> number of workers, -b -> buffersize for pipes, -i -> input directory
VAL=valgrind --leak-check=full


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE) $(FILE) $(ARGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

val:
	$(VAL) ./$(EXECUTABLE) $(FILE) $(ARGS)

script:
	./$(SCRIPT) diseasesFile.txt countriesFileXSmall.txt ../folder2 5 10