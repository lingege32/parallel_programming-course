#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <utility>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#define THREAD 8

bool checkErr(cl_int err)
{
	if(err!=CL_SUCCESS){
		std::cerr<<"error code is "<<err<<"\n";
		return 1;
	}
	return 0;
}
void setImage(char buffer[],unsigned char image[],long long int imageBeg,
			  long long int bufferBeg,long long int bufferEnd)
{
	long long int beg_=bufferBeg;
	for(long long int x=bufferBeg;x<=bufferEnd;++x)
	{
		if(buffer[x]<'0')
		{
			unsigned char rgb=0;
			for(long long int y=beg_;y<x;++y)
			{
				rgb=rgb*10+buffer[y]-'0';
			}
			image[imageBeg++]=rgb;
			beg_ = x+1;
		}
	}
}
int main(int argc, char const *argv[])
{
	const unsigned int answerSize=768; // 256 * 3
	// read file
	std::ifstream inFile("input", std::ios_base::in);
	unsigned int input_size=0;
	inFile>>input_size;
	//inFile.seekg(0,std::ios::end);
    unsigned long long int finLength = 4 * input_size;// inFile.tellg();
	char * buffer = new char[finLength];
	inFile.read(buffer,finLength);
	inFile.close();
	long long int filePos=0;
	long long int fileEnd=0;
	while(buffer[fileEnd]!='\n') ++fileEnd;
	for(long long int x=filePos;(buffer[x]!=' '&&buffer[x]!='\n');++x);
	filePos = ++fileEnd;
	unsigned char *image = new unsigned char[input_size];
	unsigned int index=0;
	long long int step = input_size/THREAD;
	long long int stepBeg = step;
	long long int offset[THREAD+1];
	offset[0] = 0;
	int offsetIndex=1;
	while(1)
	{
		++fileEnd;
		if(buffer[fileEnd]<'0')
		{
			if(++index==stepBeg)
			{
				offset[offsetIndex++] = fileEnd;
				if(offsetIndex==THREAD)
					break;
				stepBeg+=step;
			}
		}
	}
	while(1)
	{
		++fileEnd;
		if(buffer[fileEnd]<'0')
		{
			if(++index==input_size)
			{
				offset[THREAD] = fileEnd;
				break;
			}
		}
	}
	std::thread imageArray[THREAD];
	for(int i=0;i<THREAD;++i)
	{
		imageArray[i] = std::move(std::thread(setImage,buffer,image,i*step,offset[i]+1,offset[i+1]));
	}
	for(int i=0;i<THREAD;++i)
		imageArray[i].join();
	std::string kernalFunction_;
	{
		std::ifstream readCl("0650294.cl",std::ios::in);
		if(!readCl)
		{
			std::cerr<<"failed to open kernal file\n";
			return 1;
		}
		while(1)
		{
			std::string tmp;
			std::getline(readCl,tmp);
			if(readCl.eof())
				break;
			kernalFunction_+=(tmp+"\n");
		}
	}
	cl_int err;
	// Get platform ID
	cl_uint numPlatform;
	cl_platform_id	platformID_;
	err = clGetPlatformIDs(0, NULL, &numPlatform);
	if(checkErr(err)) return 1;
	err = clGetPlatformIDs(1, &platformID_,NULL);
	if(checkErr(err)) return 1;
	// Get Device ID
	cl_device_id deviceID_;
	cl_uint numDevice;
	err = clGetDeviceIDs(platformID_,CL_DEVICE_TYPE_GPU,0/*num entries*/,NULL,&numDevice);
	if(checkErr(err)) return 1;
	err = clGetDeviceIDs(platformID_,CL_DEVICE_TYPE_GPU,1/*num entries*/,&deviceID_,NULL);
	if(checkErr(err)) return 1;
	// Get context
	cl_context myctx_ = clCreateContext(0,1,&deviceID_,NULL,NULL,&err);
	if(checkErr(err)) return 1;
	// commandqueue
	cl_command_queue myqueue_ = clCreateCommandQueue(myctx_,deviceID_,
													 CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,&err);
	if(checkErr(err)) return 1;
	cl_mem deviceInput_ = clCreateBuffer(myctx_,CL_MEM_READ_ONLY,
										 input_size*sizeof(unsigned char),NULL,&err);
	if(checkErr(err)) return 1;
	cl_mem deviceOutput_ = clCreateBuffer(myctx_,CL_MEM_WRITE_ONLY,
										  768*sizeof(unsigned int),NULL,&err);
	if(checkErr(err)) return 1;
	err = clEnqueueWriteBuffer(myqueue_,deviceInput_,CL_TRUE,0,
				         	   input_size*sizeof(unsigned char),image,0,NULL,NULL);
	if(checkErr(err)) return 1;
	const size_t kernalSize = kernalFunction_.length();
	const char* kerFun_ = kernalFunction_.c_str();
	//create program
	cl_program myProg_ = clCreateProgramWithSource(myctx_,1,const_cast<const char**>(&kerFun_),
												   &kernalSize,&err);
	if(checkErr(err)) return 1;

	//build the program
	err = clBuildProgram(myProg_,1,&deviceID_,NULL,NULL,NULL);
	//if(checkErr(err)) return 1;
	// if __kernal function error
	if(err == CL_BUILD_PROGRAM_FAILURE){
		size_t log_size;
		clGetProgramBuildInfo(myProg_, deviceID_, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *) malloc(log_size);
		//
		// Get the log
		clGetProgramBuildInfo(myProg_, deviceID_, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
		delete log;
		return 0;
	}

	cl_kernel myKernel_ = clCreateKernel(myProg_,"histogram",&err);
	if(checkErr(err)) return 1;
	clSetKernelArg(myKernel_,0,sizeof(cl_mem),&deviceInput_);
	clSetKernelArg(myKernel_,1,sizeof(cl_mem),&deviceOutput_);
	clSetKernelArg(myKernel_,2,sizeof(cl_uint),&input_size);

	size_t globalWorkSize;
	size_t localWorkSize;
	err = clGetKernelWorkGroupInfo(myKernel_, deviceID_, CL_KERNEL_WORK_GROUP_SIZE,
								   sizeof(size_t),
								   &localWorkSize, NULL);
	if(checkErr(err)) return 1;
	size_t num_work_groups = (input_size + localWorkSize - 1) / localWorkSize;
	globalWorkSize = num_work_groups * localWorkSize;
	err = clEnqueueNDRangeKernel(myqueue_, myKernel_, 1, NULL,
							     &globalWorkSize, &localWorkSize, 0, NULL, NULL);
	if(checkErr(err)) return 1;

	delete[] buffer;
	delete[] image;
	unsigned int * histogram_results = new unsigned int[768];
	err = clEnqueueReadBuffer(myqueue_,deviceOutput_,CL_TRUE,0,
							  768*sizeof(unsigned int),histogram_results,0,NULL,NULL);
	if(checkErr(err)) return 1;
	//free opencl
	err = clFlush(myqueue_);
	if(checkErr(err)) return 1;
	err = clFinish(myqueue_);
	if(checkErr(err)) return 1;
	err=clReleaseKernel(myKernel_);
	if(checkErr(err)) return 1;
	err=clReleaseProgram(myProg_);
	if(checkErr(err)) return 1;
	err=clReleaseMemObject(deviceInput_);
	if(checkErr(err)) return 1;
	err=clReleaseMemObject(deviceOutput_);
	if(checkErr(err)) return 1;
	err = clReleaseCommandQueue(myqueue_);
	if(checkErr(err)) return 1;
	err = clReleaseContext(myctx_);
	if(checkErr(err)) return 1;




	std::ofstream outFile("0650294.out", std::ios_base::out);
	for(unsigned int i = 0; i < 256 * 3; ++i) {
		if (i % 256 == 0 && i != 0)
			outFile <<"\n";
		outFile << histogram_results[i]<< ' ';
	}
	delete histogram_results;


	return 0;
}
