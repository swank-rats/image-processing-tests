//============================================================================
// Name        : PerformanceTest.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process server performance testing tool entry point
//============================================================================
#include <iostream>

#include <Poco\Uri.h>
#include <Poco\Thread.h>
#include <Poco\Task.h>
#include <Poco\TaskManager.h>

#include "subsystems\StreamLatencyMeasurement.h"


int main(int argc, const char* argv[])
{
	try {
		Poco::TaskManager tm;

		StreamLatencyMeasurement* slm = new StreamLatencyMeasurement(Poco::URI("http://127.0.0.1:4711/videostream"));
		tm.start(slm);

		Poco::Thread::sleep(10000);

		tm.joinAll();
	}
	catch (Poco::SyntaxException& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cerr << "Message: " << e.message() << std::endl;
		std::cerr << e.displayText() << std::endl;
	}

	return 0;
}

