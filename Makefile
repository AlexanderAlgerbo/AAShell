CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src/AAShell.c src/loop.c src/commands.c src/logic.c
OBJ = $(SRC:.c=.o)
TARGET = AAShell

TEST_SRC = tests/tests.c src/loop.c src/logic.c src/commands.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_TARGET = tests

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) -o $@ $(TEST_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ)

.PHONY: all clean