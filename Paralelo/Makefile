CC     = gcc
CFLAGS = -Wall -Wno-unused-result -O2
LDFLAGS = -lm -lpthread
RM     = rm
#------------------------------------------------

all: aco

aco:  main.o aco.o tbarrier.o lista.o util.o
	$(CC) main.o aco.o tbarrier.o lista.o util.o -o aco $(LDFLAGS)

main.o: main.c util.h tbarrier.h aco.h
	$(CC) $(CFLAGS) -c main.c 

aco.o: aco.c aco.h lista.h tbarrier.h util.h
	$(CC) $(CFLAGS) -c aco.c 

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c 

tbarrier.o: tbarrier.c tbarrier.h
	$(CC) $(CFLAGS) -c tbarrier.c 

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c 

clean: 
	rm -f *.o 