/**********************************************************************
 * DESCRIPTION:
 *   Serial Concurrent Wave Equation - C Version
 *   This program implements the concurrent wave equation
 *********************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>

#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265
#define THREADPERWARP 32
#define SMNUM		   80

void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* number of time steps */
	tpoints, 	     		/* total points along string */
	rcode;                  	/* generic return code */
float  values[MAXPOINTS]; 	/* values at time t */
float *cudaValues;
int cudaArraySize;


/**********************************************************************
 *	Checks input values from parameters
 *********************************************************************/
void check_param(void)
{
	char tchar[20];

	/* check number of points, number of iterations */
	while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
		printf("Enter number of points along vibrating string [%d-%d]: "
				,MINPOINTS, MAXPOINTS);
		scanf("%s", tchar);
		tpoints = atoi(tchar);
		if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
			printf("Invalid. Please enter value between %d and %d\n",
					MINPOINTS, MAXPOINTS);
	}
	while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
		printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
		scanf("%s", tchar);
		nsteps = atoi(tchar);
		if ((nsteps < 1) || (nsteps > MAXSTEPS))
			printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
	}

	printf("Using points = %d, steps = %d\n", tpoints, nsteps);

}

/**********************************************************************
 *     Initialize points on line
 *********************************************************************/
void init_line(void)
{
	int i, j;
	float x, tmp;

	/* Calculate initial values based on sine curve */
	//float fac = 2.0 * PI;
	float fac = 6.2831853;
	//k = 0.0;
	tmp = tpoints - 1;
	for (j = 0; j < tpoints; ++j) {
		x = static_cast<float>(j)/tmp;
		values[j] = sin (fac * x);
	}

	/* Initialize old values array */
}
/**********************************************************************
 *     Print final results
 *********************************************************************/
void printfinal()
{
	int i;

	printf("0.0000 ");
	for (i = 1; i < tpoints; ++i) {
		printf("%6.4f ", values[i]);
		if (i%10 == 9)
			printf("\n");
	}
}
__global__ void cudaExecute(float* cudaAns,int howMany,int tpoints,int tIteration)
{
	float cudaValues,cudaOld,cudaNew;
	double cudaTwiceValue;
	double valuePar = 2.0 - static_cast<float>(0.09) * 2.0;
	for(int block=0;block<howMany;++block)
	{
		int ansIndex=block*(SMNUM*THREADPERWARP) + blockIdx.x*blockDim.x + threadIdx.x;
		cudaValues = cudaAns[ansIndex];
		cudaOld = cudaValues;
		for(int iter=0;iter<tIteration;++iter)
		{
			cudaTwiceValue = valuePar * cudaValues;
			cudaNew = (cudaTwiceValue) - cudaOld;
			//cudaNew = (2.0 * cudaValues)
			//				 - cudaOld
			//				 + (static_cast<float>(0.09) *  (-2.0)*cudaValues);
			cudaOld=cudaValues;
			cudaValues=cudaNew;
		}
		cudaAns[ansIndex]=cudaValues;
	}
}

/**********************************************************************
 *	Main program
 *********************************************************************/
int main(int argc, char *argv[])
{
	sscanf(argv[1],"%d",&tpoints);
	sscanf(argv[2],"%d",&nsteps);
	check_param();
	int howManyBlock=tpoints/(SMNUM*THREADPERWARP);
	if(tpoints%(SMNUM*THREADPERWARP)!=0)
		++howManyBlock;
	cudaArraySize = tpoints*sizeof(float);
	printf("Initializing points on the line...\n");
	cudaMalloc((void**)&cudaValues,cudaArraySize);
	init_line();
	cudaMemcpy(cudaValues,values,cudaArraySize,cudaMemcpyHostToDevice);
	cudaExecute<<<SMNUM,THREADPERWARP>>>(cudaValues,howManyBlock,tpoints,nsteps);
	cudaMemcpy(values,cudaValues,cudaArraySize-4,cudaMemcpyDeviceToHost);
	//update();
	printf("Updating all points for all time steps...\n");
	printf("Printing final results...\n");
	printfinal();
	printf("\nDone.\n\n");

	return 0;
}
