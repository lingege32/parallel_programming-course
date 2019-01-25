#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
class parserC
{
	private:
		const char* const   mFileName_;
		std::vector<double> mPcaData_;
		int					mCol_;
		int					mRow_;
	private:
		void runParse();
	public:
		parserC()=delete;
		parserC(const char* const );
		~parserC();

		void dump();
		auto row() const -> int;
		auto col() const -> int;
		auto data() -> std::vector<double>&&; //pointer become nullptr
};
#endif
