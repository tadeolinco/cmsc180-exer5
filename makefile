run:
	mpicc problem1.c
	mpirun -n 4 ./a.out 5000