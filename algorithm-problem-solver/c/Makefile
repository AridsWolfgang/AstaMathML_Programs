CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -I src
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = build/algorithm_solver.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o build\*.exe 2>nul
	rm -f src/*.o build/algorithm_solver

run: $(TARGET)
	$(TARGET)
