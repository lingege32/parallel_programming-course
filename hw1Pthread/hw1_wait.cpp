#include<iostream>
#include<pthread.h>
#include<cstdlib>
#include<ctime>
#include<limits>

long threadNum;
long number_of_tosses;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long totalNumInCir=0;
unsigned int initialSeed(123);
volatile long counter=0;
class myRand
{
	private:
		unsigned int now;
	public:
		myRand(const unsigned int& seed):now(seed){}
		double operator()(void){
			now = now*1103515245 + 12345;
			return (static_cast<double>(now))/std::numeric_limits<unsigned int>::max();
		}
};
void* thread_sum(void* myRank)
{
	long mRank = *static_cast<long*>(myRank);
	long my_n  			  = number_of_tosses / threadNum;
	unsigned int seed = initialSeed + mRank*my_n ;
	long number_in_circle = 0;
	double xCoordinate,
		   yCoordinate;
	double disFromOrigin_ = 0.0;
	myRand myRand_(seed);

	for(long toss=0 ; toss<my_n ; ++toss)
	{
		xCoordinate=myRand_();
		yCoordinate=myRand_();

		disFromOrigin_ = (xCoordinate * xCoordinate + yCoordinate * yCoordinate);
		if(disFromOrigin_<=1.0)
			++number_in_circle;
	}
	while(counter!=mRank);
	totalNumInCir += number_in_circle;
	counter = (counter+1) % threadNum;
	return nullptr;
}

int main(int argc,char** argv)
{
	if(argc!=3){
		std::cout<<"wrong parameter#\n";
		std::cout<<"./main <#of cpu cores> <numberOfTosses>\n";
		return 0;
	}

	threadNum = std::atol(argv[1]);
	number_of_tosses=std::atol(argv[2]);
	pthread_t *thread_handles;
	thread_handles = new pthread_t[threadNum];


	for(long threadId = 0; threadId < threadNum; ++threadId){
		long* id = new long(threadId);
		pthread_create(&thread_handles[threadId],nullptr,thread_sum,id);
	}

	for(long threadId = 0; threadId < threadNum; ++threadId)
		pthread_join(thread_handles[threadId],nullptr);

	double piAns = static_cast<double>(totalNumInCir)/number_of_tosses;
	std::cout<<"pi is "<<4.0*piAns<<"  error: \033[1;31m"<<std::scientific<< (((4.0*piAns - 3.1415926)>0.0)?(4.0*piAns - 3.1415926):(-(4.0*piAns - 3.1415926)))
														   <<"\033[0m"<<std::endl;
	return 0;
}
