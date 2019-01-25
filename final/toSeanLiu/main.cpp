#include"parser.hpp"
#include"cnnModel.hpp"
#include<iostream>

int main(int argc,char* argv[])
{
	if(argc==1)
	{
		std::cout<<"no input\n";
		return 0;
	}
	parserC* parseObj = new parserC{argv[1]};
	//parseObj->dump();
	cnnModelC modelObj{parseObj};
	//parseObj->dump();
	//modelObj.showPcaData();
	modelObj.modelTraining();
	modelObj.modelTesting();
	delete parseObj;
	return 0;
}
