#include<iostream>
#include"omp.h"

double areaOfrectangle(double x);

int main(int argc,char** argv)
{
	if(argc!=3){
		std::cout<<"need the parameters\n";
		std::cout<<"./main threadNum iteratorNum\n";
		return 1;
	}
	int threadNum = std::atoi(argv[1]);
	int stepNum  = std::atoi(argv[2]);
	double step = 1.0/stepNum;
	omp_set_num_threads(threadNum);
	double* sum = new double[threadNum];
	#pragma omp parallel
	{
		int threadID = omp_get_thread_num();
		int nthThread = omp_get_num_threads();
		if(threadID == 0)
			threadNum = nthThread;
		int begin = threadID ;
		int end   = stepNum;
		double xCoor;
		sum[threadID]=0.0;
		for(; begin<end; begin+=nthThread)
		{
			xCoor = (begin+0.5)*step;
			sum[threadID] += areaOfrectangle(xCoor);
		}
	}
	double pi=0.0;
	for(int x=0;x<threadNum;++x)
		pi+=sum[x]*step;
	std::cout<<pi<<std::endl;
	return 0;
}
double areaOfrectangle(double x)
{
	return 4.0/(1+x*x);
}
