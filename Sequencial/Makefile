CC     = gcc
CFLAGS = -Wall -Wno-unused-result
LDFLAGS = -lm
RM     = rm
#------------------------------------------------

all: aco

aco:  main.o aco.o util.o
	$(CC) main.o aco.o util.o -o aco $(LDFLAGS)

main.o: main.c util.h aco.h
	$(CC) $(CFLAGS) -c main.c 

aco.o: aco.c aco.h util.h
	$(CC) $(CFLAGS) -c aco.c 

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c 

clean: 
	rm -f *.o 