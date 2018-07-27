CCC=gcc 
CFLAGS= -lm -g -std=c99

all: arkinator

arkinator: arkinator.o


clean:
	rm -f arkinator
