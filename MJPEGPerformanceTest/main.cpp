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
#include <Poco\Exception.h>

#include "StreamLatencyMeasurement.h"

using std::cerr;
using Poco::URI;
using Poco::TaskManager;
using Poco::Thread;
using Poco::Exception;

int main(int argc, const char* argv[])
{
	try {
		TaskManager tm;

		StreamLatencyMeasurement* slm = new StreamLatencyMeasurement(URI("http://127.0.0.1:4711/videostream"));
		tm.start(slm);

		Thread::sleep(10000);

		tm.joinAll();
	}
	catch (Exception& e) {
		cerr << e.displayText() << std::endl;
	}

	return 0;
}

