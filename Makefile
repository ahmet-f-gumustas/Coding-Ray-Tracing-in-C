CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = `sdl2-config --cflags --libs` -lm
TARGET = raytracer
SOURCES = main.c raytracer.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run