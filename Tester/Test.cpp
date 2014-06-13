#include <chrono>
#include <string>
#include "Jupiter/Functions.h"
#include "Jupiter/CString.h"
#include "Jupiter/String.h"
#include "Jupiter/File.h"
#include "Jupiter/Base64.h"
#include "Jupiter/InvalidIndex.h"
#include "Jupiter/Reference_String.h"

using namespace Jupiter;

unsigned int goodTests = 0;
unsigned int totalTests = 0;

void test(bool expr)
{
	totalTests++;
	if (expr) goodTests++;
	else printf("Test number %u failed!" ENDL, totalTests);
}

int main()
{
	if (goodTests == totalTests) printf("All %u tests succeeded." ENDL, totalTests);
	else printf("ERROR: Only %u/%u tests succeeded. %u tests failed." ENDL, goodTests, totalTests, totalTests - goodTests);
	puts("Press any key to continue...");
	fgetc(stdin);
	return 0;
}