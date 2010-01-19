#include "easylogger.h"

int main() {
	easylogger::Logger TEST("TEST");
	easylogger::Logger SUB("SUB", TEST);
	SUB.Level(easylogger::LEVEL_WARNING);

	LOG_INFO(TEST, "Hi!" << 42);
	LOG_DEBUG(TEST, "don't show me");
	LOG_INFO(SUB, "info!");
	LOG_ERROR(SUB, "error!");
	LOG_FATAL(TEST, "dead");
	LOG_ERROR(TEST, "won't see me");
	return 0;
}
