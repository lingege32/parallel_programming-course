#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(int n) {
	int i,squareroot;
	if (n>10) {
		squareroot = (int) sqrt(n);
		for (i=3; i<=squareroot; i=i+2)
			if ((n%i)==0)
				return 0;
		return 1;
	}
	else
	return 0;
}

int main(int argc, char *argv[])
{
	int size,rank;
	MPI_Init(&argc,&argv);
	int pc,       /* prime counter */
	    foundone; /* most recent prime found */
	int totalPC,maxPrime;
	long long int n, limit;

	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0){
		sscanf(argv[1],"%llu",&limit);
		printf("Starting. Numbers to be scanned= %lld\n",limit);
	}
	MPI_Bcast(&limit,
			  1,
			  MPI_LONG_LONG,
			  0,
			  MPI_COMM_WORLD);
	pc=0;     /* Assume (2,3,5,7) are counted here */

	int step=(size<<1);


	for (int n=11+2*rank; n<=limit; n+=step) {
		if (isprime(n)) {
			pc++;
			foundone = n;
		}
	}

	MPI_Reduce(&pc,
			   &totalPC,
			   1,
			   MPI_INT,
			   MPI_SUM,
			   0,
			   MPI_COMM_WORLD);
	MPI_Reduce(&foundone,
			   &maxPrime,
			   1,
			   MPI_INT,
			   MPI_MAX,
			   0,
			   MPI_COMM_WORLD);

	MPI_Finalize();
	if(rank==0)
		printf("Done. Largest prime is %d Total primes %d\n",maxPrime,totalPC+4);

	return 0;
}
