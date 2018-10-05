BUILDDIR=build
BINDIR=bin

CFLAGS=-save-temps=obj -g -O0 -Wall -Isrc -Iinclude -Ilibsrc $(OPTFLAGS) -Wno-unused-variable -Wno-null-dereference -std=c++17 -DDEBUG
LDLIBS=-Llib -L$(BUILDDIR) -L$(BINDIR) $(OPTLIBS)
LDPATH=-Iinclude -Ibuild -Lbin
PREFIX?=/usr/local
CC=clang++
AR=llvm-ar
RANLIB=llvm-ranlib


SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst src/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

LIBSOURCES=$(wildcard libsrc/**/*.cpp libsrc/*.cpp)
LIBOBJECTS=$(patsubst libsrc/%.cpp,$(BUILDDIR)/%.o,$(LIBSOURCES))

SOURCES2=$(wildcard src2/**/*.cpp src2/*.cpp)
S2OBJECTS=$(SOURCES2:src2/%.cpp=$(BUILDDIR)/%.o)
EXECUTABLE_FILES= $(SOURCES2:src2/%.cpp=$(BINDIR)/%)

TEST_SRC=$(wildcard tests/*_tests.cpp)
TEST_OBJ=$(patsubst %.cpp,%.o,$(TEST_SRC))
TESTS=$(patsubst %.cpp,%,$(TEST_SRC))

TARGET=bin/advent.exe
SO_LIB=advent
SO_TARGET=bin/lib$(SO_LIB).a

# The Target Build
all: build $(TARGET) $(EXECUTABLE_FILES)

dev: CFLAGS=-g -Wall -Isrc -Iinclude -Wall -Wextra $(OPTFLAGS)
dev: all

$(EXECUTABLE_FILES): bin/%: build/%.o
	$(CC) $^ $(LDLIBS) -l$(SO_LIB) -o $@

$(S2OBJECTS): $(BUILDDIR)/%.o: src2/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

$(TARGET): build $(SO_TARGET) $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) -l$(SO_LIB) -o $@

$(SO_TARGET): $(LIBOBJECTS)
	$(AR) ru $@ $^
	$(RANLIB) $@

#%.o : %.cpp
$(LIBOBJECTS): build/%.o: libsrc/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJECTS): build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

build:
	@mkdir -p build
	@mkdir -p bin

run: $(TARGET)
	@build/advent.exe

# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(SO_TARGET)
tests: LDLIBS += -ladvent
tests: $(SO_TARGET) $(TEST_OBJ) $(TESTS)
	@echo Test Source: $(TEST_SRC)
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
