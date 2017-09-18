
Joshua Rodstein
Project 3
readme




   Program seems to have memory leak when tested with the provided mallocdrv. My test program simply allocates
3 times, and frees in a different order, which seems to be in perfect working order. 

compiled with following command: gcc -Wall -Werror -m32 -g -o mallocdrv mallocdrv.c mymalloc.c


