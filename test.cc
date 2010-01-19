#include "easylogger.h"

static easylogger::Logger TEST("TEST");
static easylogger::Logger SUB("SUB", TEST);

static void test1() {
	EASY_TRACE(TEST, test1);

	LOG_INFO(TEST, "Hi!" << 42);
}

static void test2() {
	EASY_TRACE(TEST, test1);

	LOG_DEBUG(TEST, "don't show me");
}

int main() {
	TEST.Level(easylogger::LEVEL_TRACE);
	SUB.Level(easylogger::LEVEL_WARNING);

	EASY_TRACE(TEST, main);

	test1();
	test2();

	LOG_INFO(SUB, "info!");
	LOG_ERROR(SUB, "error!");
	//LOG_FATAL(TEST, "dead");
	//LOG_ERROR(TEST, "won't see me");

	return 0;
}
