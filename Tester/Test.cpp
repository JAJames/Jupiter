#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include "Jupiter/Functions.h"
#include "Jupiter/CString.h"
#include "Jupiter/String.h"
#include "Jupiter/File.h"
#include "Jupiter/Base64.h"
#include "Jupiter/InvalidIndex.h"
#include "Jupiter/Reference_String.h"
#include "Jupiter/DataBuffer.h"
#include "Jupiter/HTTP.h"
#include "Jupiter/HTTP_Server.h"
#include "Jupiter/HTTP_QueryString.h"
#include "Jupiter/Hash.h"
#include "Jupiter/Hash_Table.h"
#include "Jupiter/Algorithm.h"

using namespace Jupiter;
using namespace Jupiter::literals;

unsigned int good_tests = 0;
unsigned int total_tests = 0;

void test(bool expr)
{
	++total_tests;
	if (expr)
		++good_tests;
	else
		std::cout << "Test number " << total_tests << " failed!" << std::endl;
}

template<typename T> void test(T result, T expected_result)
{
	++total_tests;

	if (result == expected_result)
		++good_tests;
	else
	{
		std::cout << "Test number " << total_tests << " failed!" << std::endl;
		std::cout << "\tExpected '" << expected_result << "' but got '" << result << "' instead" << std::endl;
	}
}

int main()
{
	if (good_tests == total_tests)
		std::cout << "All " << total_tests << " tests succeeded." << std::endl;
	else
		std::cout << "ERROR: Only " << good_tests << "/" << total_tests << " tests succeeded. " << total_tests - good_tests << " tests failed." << std::endl;

	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();

	return 0;
}
