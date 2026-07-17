CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -I src
SRC = $(wildcard src/*.c)
TARGET = build/custom_math.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	del /Q build\*.exe 2>nul

run: $(TARGET)
	$(TARGET)
