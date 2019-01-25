#include<iostream>
#include<iomanip>
#include"omp.h"
#define PI 3.14159265359

int main()
{
	if(argc!=2){
		std::cout<<"need the parameters\n";
		std::cout<<"./main threadNum \n";
		return 1;
	}
	int threadNum = std::atoi(argv[1]);
	omp_set_num_threads(threadNum);
	double pi=0;
	double sign = 1.0;
	unsigned long    iter = 0;
	unsigned long     para = 0;
	while(1)
	{
		para = iter*2+1;
		pi+=(sign/static_cast<double>(para));
		//std::cout<<std::setprecision(10)<<4*pi<<"\n";
		//std::cout<<iter<<"\n";
		//getchar();
		sign = -sign;
		++iter;
		if(4*pi-3.14159265359<=0.000000001 && 4*pi-3.14159265359>=-0.000000001)
			break;
	}
	std::cout<<"pi: "<<std::setprecision(20)<<4*pi<<"  iter: "<<iter<<std::endl;
	return 0;
}
