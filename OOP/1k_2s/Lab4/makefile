CC=g++
CFLAGS=-g -c -Wall
LDFLAGS=
SOURCES=lab4.cpp lab4_classes.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=lab4

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE).exe