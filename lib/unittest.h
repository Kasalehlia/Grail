// vim: set noexpandtab:

#ifndef UNITTEST_H
#define UNITTEST_H

#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include <list>
#include <map>
#include <cstdio>
#include <cstdarg>

#include "utils.h"

namespace grail {

class Unittest {
		enum { INFO_BUFSIZE = 1024 };
		
		static std::map<std::string, std::list<Unittest*> > tests;
		static char info_[INFO_BUFSIZE];
		
	protected:
		std::string group, name;
		std::ostream& out;
		
		struct TestResult {
			size_t passed;
			size_t failed;
			TestResult() : passed(0), failed(0) { };
			void clear() { passed = failed = 0; }
		};
		
	public:
		
		Unittest(std::string group, std::string name);
		virtual void run(TestResult& result) = 0;
		void checkEqual(bool r, std::string v1, std::string s1, std::string s2, TestResult& result, std::string file, size_t line);
		void checkGreater(bool r, std::string v1, std::string s1, std::string s2, TestResult& result, std::string file, size_t line);
		void checkLower(bool r, std::string v1, std::string s1, std::string s2, TestResult& result, std::string file, size_t line);
		void info(const char* fmt, ...);
		void popInfo(bool show);
		
		static void runAll();
		
};

#define TEST(GROUP, TEST) class __unittest__ ## GROUP ## __ ## TEST : public Unittest { \
	public: \
	__unittest__ ## GROUP ## __ ## TEST () : Unittest(#GROUP, #TEST) { }; \
	void run(TestResult& result); \
	}  unittest ## GROUP ## TEST ## Instance; \
	void __unittest__ ## GROUP ## __ ## TEST :: run(Unittest::TestResult& result)

#define CHECK_EQUAL(x, y) checkEqual((x) == (y), toString((x)), #x, #y, result, __FILE__, __LINE__)

#define CHECK_GREATER(x, y) checkGreater((x) > (y), toString((x)), #x, #y, result, __FILE__, __LINE__)

#define CHECK_LOWER(x, y) checkLower((x) < (y), toString((x)), #x, #y, result, __FILE__, __LINE__)

} // namespace grail

#endif // UNITTEST_H

