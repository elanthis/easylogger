test-bin: test.cc easylogger.h easylogger-impl.h Makefile
	$(CXX) -g -o test-bin test.cc
	./test-bin
