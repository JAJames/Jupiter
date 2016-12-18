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

using namespace Jupiter;
using namespace Jupiter::literals;

unsigned int goodTests = 0;
unsigned int totalTests = 0;

void test(bool expr)
{
	++totalTests;
	if (expr)
		++goodTests;
	else
		printf("Test number %u failed!" ENDL, totalTests);
}

Jupiter::StringS randstr(size_t length)
{
	StringS str;

	while (length != 0)
	{
		str += ' ' + rand() % ('z' - ' ');
		--length;
	}

	return str;
}

int main()
{
	if (goodTests == totalTests)
		printf("All %u tests succeeded." ENDL, totalTests);
	else
		printf("ERROR: Only %u/%u tests succeeded. %u tests failed." ENDL, goodTests, totalTests, totalTests - goodTests);

	puts("Press any key to continue...");
	fgetc(stdin);

	return 0;
}
