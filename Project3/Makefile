CC= gcc

CFLAGS= -m32

TARG= malloctest

$(TARG): mymalloc.o mallocdrv.c
	gcc $(CFLAGS) -o malloctest mallocdrv.c mymalloc.c

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) -c mymalloc.c

mallocdrv.o: mymalloc.h mallocdrv.c
	$(CC) -c mallocdrv.c

.PHONY: cleanobj
cleanobj:
	-rm *.o

.PHONY: clean
cleanall:
	-rm $(TARG) *.o 


	



