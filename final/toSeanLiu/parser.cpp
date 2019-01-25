#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
/**************************************************
 * parserC::parserC()
 **************************************************/

parserC::parserC(const char* const fileName): mFileName_{fileName}
{
	runParse();
}
parserC::~parserC()
{
	std::cout<<"parser destructor\n";
}


/**************************************************
 * parserC::runParse()
 **************************************************/
void parserC::runParse()
{
	std::ifstream fin(mFileName_,std::ios::in);
	std::string  tmp;
	std::stringstream sstr;
	double elementTmp;
	int row{0},col{0};
	while(1)
	{
		sstr.clear();
		tmp.clear();
		std::getline(fin,tmp);
		if(fin.eof())
			break;
		row++;
		sstr<<tmp;
		col=0;
		while(sstr>>elementTmp)
		{
			++col;
			mPcaData_.emplace_back(elementTmp);
		}
	}
	mCol_=col;
	mRow_=row;
}
/**************************************************
 * parserC::dump()
 **************************************************/
void parserC::dump()
{
	std::string dataInfo{std::string{"Total elements#: "}+std::to_string(mPcaData_.size())+
						 std::string{" Row#: "}+std::to_string(mRow_)+
						 std::string{" Col#: "}+std::to_string(mCol_)};
	std::cout<<dataInfo<<std::endl;
	std::cout<<"pointer: "<<mPcaData_.data()<<std::endl;
	std::cout<<"vector pointer: "<<&mPcaData_<<std::endl;
	std::cout<<"capacity: "<<mPcaData_.capacity()<<std::endl;
	//for(size_t x=0;x<mPcaData_.size();++x){
	//	std::cout<<std::fixed<<std::setprecision(6)<<mPcaData_[x];
	//	if(static_cast<int>(x)%mCol_==(mCol_-1))
	//		std::cout<<"\n";
	//	else
	//		std::cout<<" ";
	//}
	//std::cout<<std::endl;
}

auto parserC::row() const -> int {return mRow_;}
auto parserC::col() const -> int {return mCol_;}
auto parserC::data() -> std::vector<double>&&
{
	return std::move(mPcaData_);
}
