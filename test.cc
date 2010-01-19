#include "easylogger.h"

static easylogger::Logger TEST("TEST");
static easylogger::Logger TRACE("TRACE");
static easylogger::Logger SUB("SUB", TEST);

static void test1() {
	EASY_TRACE(TRACE, test1);

	LOG_INFO(TEST, "Hi!" << 42);
}

static void test2() {
	EASY_TRACE(TRACE, test1);

	LOG_DEBUG(TEST, "don't show me");
}

int main() {
	SUB.Level(easylogger::LEVEL_WARNING);
	TRACE.Format("[%F:%C %P] %N: %S");
	TRACE.Level(easylogger::LEVEL_TRACE);

	EASY_TRACE(TRACE, main);

	test1();
	test2();

	LOG_INFO(SUB, "info!");
	LOG_ERROR(SUB, "error!");
	//LOG_FATAL(TEST, "dead");
	//LOG_ERROR(TEST, "won't see me");

	return 0;
}
