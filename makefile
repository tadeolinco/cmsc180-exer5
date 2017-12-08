run:
	mpicc finals.c
	mpirun -np 4 ./a.out 5