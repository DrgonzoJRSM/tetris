CC = gcc

CFLAGS = -Wall -Wextra

LDFLAGS = -lncurses

TARGET = game

SRC = main.c parse.c tetris.c graphics.c
HEADERS = includes.h tetris.h graphics.h parse.h 

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY:
	all clean
