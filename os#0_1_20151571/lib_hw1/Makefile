CC = gcc

CFLAG = -W -Wall -g
SOURCES = list.c bitmap.c hash.c main.c
OBJECTS = $(SOURCE:.c=.o)

TARGET = a.out

all : $(SOURCES) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm *.o $(TARGET)
