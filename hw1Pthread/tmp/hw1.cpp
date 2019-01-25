#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

using namespace std;
void* Thread_Toss(void* rank);
pthread_mutex_t mutex;

int p_nNumOfToss;
int p_nThreadCount;
double p_nNumInCircle;
int p_nNumOfLoop;

class myRand{
    public:
        myRand(const unsigned int N):my_next(N){};
        double operator()(void){
            my_next = my_next*1103515245 + 12345;
            return (static_cast<double>(my_next)/4294967294);
        }
    private:
        unsigned int my_next;
};

int main(int argc,char** argv){
    double p_dPi;
    long thread;
    pthread_t* thread_handles;
    p_nThreadCount = strtol(argv[1], NULL, 10);
    p_nNumOfToss = strtoll(argv[2], NULL, 10);

    thread_handles = (pthread_t*) malloc (p_nThreadCount*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    p_nNumInCircle = 0;
    p_nNumOfLoop = p_nNumOfToss/p_nThreadCount;
   
    for(thread=0 ; thread<p_nThreadCount ; ++thread){
        pthread_create(&thread_handles[thread], NULL, Thread_Toss, (void*)thread);
    }
    for(thread=0 ; thread<p_nThreadCount ; ++thread){
        pthread_join(thread_handles[thread], NULL);
    }

    p_dPi = 4*p_nNumInCircle / ( (double) p_nNumOfToss );
    cout<<p_dPi<<endl;

    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
}

void* Thread_Toss(void* rank){

    long my_rank = (long) rank;
    int  toss;
    long my_n = p_nNumOfToss/p_nThreadCount;
    long my_first_toss = my_n*my_rank;
    long my_last_toss  = my_first_toss+my_n;
    unsigned int  my_NumInCircle = 0;
    double p_nX,p_nY;
    double p_nDistance;
    myRand RandNum(my_rank);
    //static unsigned long int my_next = my_rank;

    for(toss=my_first_toss ; toss < my_last_toss ; ++toss){
        p_nX = RandNum();
        p_nY = RandNum();
        // my_next = my_next * 1103515245 + 12345;
        // p_nX = static_cast<double> ((unsigned int)my_next)/4294967294;
        // my_next = my_next * 1103515245 + 12345;
        // p_nY = static_cast<double> ((unsigned int)my_next)/4294967294;
        p_nDistance = p_nX*p_nX + p_nY*p_nY;
        if(p_nDistance <= 1) my_NumInCircle++;
    }
    pthread_mutex_lock(&mutex);
    p_nNumInCircle += my_NumInCircle;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

