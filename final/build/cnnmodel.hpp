#ifndef CNNMODEL_HPP
#define CNNMODEL_HPP
class cnnmodelC
{
	private:
		std::vector<double> mPcaData_;
		int	    mPcaRow_;
		int	    mPcaCol_;
	public:
		cnnmodelC()=delete;
		cnnmodelC(parserC*);
};

#endif
