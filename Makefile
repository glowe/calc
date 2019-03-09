CC := /usr/local/opt/llvm/bin/clang
LDFLAGS := -L/usr/local/opt/llvm/lib
CPPFLAGS := -I/usr/local/opt/llvm/include
CFLAGS := -g --std=c99 -Wall -Wextra -Werror -Wno-unused-parameter
HEADERS := $(wildcard src/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

build/calc: $(OBJECTS) 
	$(CC) $(LDFLAGS) $^ -o $@

build/%.o: src/%.c $(HEADERS) build
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

build:
	mkdir build

clean:
	-rm -rf build

.PHONY: clean
