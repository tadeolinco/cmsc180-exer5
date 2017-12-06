run:
	mpicc problem2.c
	mpirun -n 4 ./a.out 1000