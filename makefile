simpleshell_c: simpleshell.o
	gcc -o simpleshell_c simpleshell.o

simpleshell.o: simpleshell.c
	gcc -c simpleshell.c

clean:
	rm *.o simpleshell_c