sched: sched.o
	gcc -o sched sched.o -pthread

sched.o: sched.c
	gcc -c -Wall sched.c

clean:
	rm *.o sched