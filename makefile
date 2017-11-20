run:
	mpicc hello.c
	mpirun -np 4 ./a.out 10000