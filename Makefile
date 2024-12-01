CFLAGS = -Wall -Wextra -pedantic
ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

SRC = $(wildcard src/*.c)
TGT = $(patsubst src/%.c,build/%,$(SRC))

all: $(TGT)

build/%: src/%.c
	@mkdir -p build/
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf ./build

.PHONY: all clean
