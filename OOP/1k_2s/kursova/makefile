CC=g++
CFLAGS=-g -c -Wall
LDFLAGS=
SOURCES=classes.cpp kursova.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=kursova

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE).exe