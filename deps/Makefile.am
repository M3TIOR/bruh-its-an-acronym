
deps/munit.o:
	$(CC) -o deps/munit.o deps/munit.c

all: deps/munit.o
	$(cd argparse && $(MAKE) $(AM_MAKEFLAGS) argparse.o)

