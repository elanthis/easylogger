all: docs test-bin

test-bin: test.cc easylogger.h easylogger-impl.h Makefile
	$(CXX) -g -o test-bin test.cc
	./test-bin

docs:
	doxygen

clean:
	rm -f test-bin
