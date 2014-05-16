#include <chrono>
#include <string>
#include "../Jupiter/Functions.h"
#include "../Jupiter/CString.h"
#include "../Jupiter/File.h"
#include "../Jupiter/Base64.h"
#include "../Jupiter/InvalidIndex.h"

const char *tString = "ZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMGZOMG";
const char *tStringL = "zomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomgzomg";

Jupiter::CStringL lString = "message message message message message message message message message message message message message message message \r\n\r\nmessage message message message message message \rmessage message message\n message message message message message message message message message message message message message message message message message message message message message message message message message message message message message ";

Jupiter::CStringL tString2;

int test()
{
	return Jupiter::strlen(tString2.c_str());
	//return strmatch("ZOMG ?SS CHEEKS! LOLOLOLOL", "ZOMG ASS CHEEKS! LOLOLOLOL");
}

int test2()
{
	return ::strlen(tString2.c_str());
	//return strmatchi("ZOMG ?SS CHEEKS! LOLOLOLOL", "ZOMG ASS CHEEKS! LOLOLOLOL");
}

char *randgen(size_t size)
{
	char *r = new char[size + 1];
	r[size] = 0;
	while (size > 0)
	{
		size--;
		r[size] = rand() % 255 + 1;
	}
	return r;
}

using namespace Jupiter;

int main()
{
	printf("%u - %u - %llu" ENDL, INVALID_INDEX, INVALID_INDEX32, INVALID_INDEX64);
	printf("%u - %u - %llu" ENDL, UINT_MAX, UINT32_MAX, UINT64_MAX);

#if defined _WIN32
	system("pause");
#endif // _WIN32
	return 0;
}