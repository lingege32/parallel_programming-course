#ifndef CNNMODEL_HPP
#define CNNMODEL_HPP
#include "parser.hpp"
class cnnModelC
{
	private:
		int mIteration_;
		int mTrainSize_;
		int mRow_;
		int mCol_;
		std::vector<double> mPcaData_;
		int mNumOfLev1_;
		int mNumOfLev2_;
		double* mWeightVec1_;
		double* mWeightVec2_;
		double* mWeightVec3_;
	public:
		cnnModelC()=delete;
		cnnModelC(parserC*);
		~cnnModelC();

	public:
		void showPcaData();
		void showMatrix(double*,int,int);
		void modelTraining();
		void modelTesting();


	private:
		void initInputData(int,int,double*);
		void sigmoidFun(double*,int);
		void sigmaExp(double*,int);
		void transferOutput(double*,int);
		void getDerived(double*,double*,int);
		bool compareMatrix(double* ,double*,int);
	private: // matrix operation function
		void matrixMultiply(double*,double*,double*,int,int,int);
		void matrixMinus(double*,double*,double*,int);
		void matrixAssign(double*,double*,int,int);
		void dotMultiply(double*,double*,double*,int);
};

#endif
