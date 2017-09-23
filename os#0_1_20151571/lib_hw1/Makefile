CC=gcc

CFLAG=-W -Wall -g
SOURCES=list.c bitmap.c hash.c main.c
OBJECTS=$(SOURCES:.c=.o)

TARGET=testlib

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) #(OBJECTS) -o $@

.c.o:
		$(CC) -c $< -o $@ $(CFLAGS)

clean:
		rm *.o $(TARGET)

