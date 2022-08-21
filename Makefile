CC = cc

PNAME = nserv

SRC  = $(shell find src -name "*.c")
OBJ  = $(SRC:.c=.o)
BIN = build

EXEC = $(BIN)/$(PNAME)
INCFLAGS  = -Isrc/

CCFLAGS += $(INCFLAGS)
CCFLAGS += -O0
CCFLAGS += -std=c99
CCFLAGS += -Wall
CCFLAGS += -pedantic
CCFLAGS += -D_POSIX_C_SOURCE=199309L

LDFLAGS  = $(INCFLAGS)
LDFLAGS += -lpthread
LDFLAGS += -ldebug

INSTALL_PATH = /usr/local/bin

all: build

run: build
	$(BIN)/$(PNAME) $*

build: $(OBJ)
	$(CC) $(CCFLAGS) -ggdb -o $(BIN)/$(PNAME) $(filter %.o,$^) $(LDFLAGS)

clean:
	rm $(BIN)/* $(OBJ) logs.txt

install: build
	cp $(BIN)/$(PNAME) $(INSTALL_PATH)

%.o: %.c
	$(CC) -ggdb -o $@ -c $< $(CCFLAGS)

