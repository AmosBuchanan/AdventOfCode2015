CFLAGS=-g -O0 -Wall -Isrc -Iinclude -Ilibsrc $(OPTFLAGS) -Wno-unused-variable -Wno-null-dereference -std=c++11 -DNDEBUG
LIBS=-Llib -Lbuild -ladvent $(OPTLIBS)
PREFIX?=/usr/local
CC=clang++
AR=llvm-ar
RANLIB=llvm-ranlib

SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

LIBSOURCES=$(wildcard libsrc/**/*.cpp libsrc/*.cpp)
LIBOBJECTS=$(patsubst %.cpp,%.o,$(LIBSOURCES))

TEST_SRC=$(wildcard tests/*_tests.cpp)
TESTS=$(patsubst %.cpp,%,$(TEST_SRC))

TARGET=build/advent.exe
SO_TARGET=build/libadvent.a

# The Target Build
all: build $(TARGET)

dev: CFLAGS=-g -Wall -Isrc -Iinclude -Wall -Wextra $(OPTFLAGS)
dev: all


$(TARGET): build $(SO_TARGET) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

$(SO_TARGET): $(LIBOBJECTS)
	$(AR) ru $@ $^
	$(RANLIB) $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@ 

build:
	@mkdir -p build
	@mkdir -p bin

run: $(TARGET)
	@build/test.exe

# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(TARGET)
tests: $(TESTS)
	sh ./tests/runtests.sh

valgrind:
       VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS) $(LIBOBJECTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true


# clang++ -g -O0 -Wall -Isrc -Iinclude -DNDEBUG  -Wno-unused-variable -Wno-null-dereference -std=c++11 -DDEBUG src/adventofcode.cpp -c -o src/adventofcode.o 
# clang++ -g -O0 -Wall -Isrc -Iinclude -DNDEBUG  -Wno-unused-variable -Wno-null-dereference -std=c++11 -DDEBUG src/adventofcode.cpp -c -o src/adventofcode.o 

# clang++ src/adventofcode.o src/day1.o src/day2.o src/day3.o src/day4.o src/day5.o src/day6.o src/helpers.o src/md5.o -Llib    -o build/test.exe

