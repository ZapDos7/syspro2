CC=g++ -std=c++11
CFLAGS=-c -Wpedantic -Ofast -Wall
SCRIPT = create_infiles.sh
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=
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
	./$(SCRIPT) diseasesFile.txt countriesFileSmall.txt ../folder 5 10