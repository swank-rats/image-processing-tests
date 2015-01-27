//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : MJPEG performance test
//============================================================================
#include <iostream>

#include <Poco\Uri.h>
#include <Poco\Thread.h>
#include <Poco\Task.h>
#include <Poco\TaskManager.h>
#include <Poco\Exception.h>
#include <Poco\Util\ServerApplication.h>
#include <Poco\Util\OptionSet.h>

#include "StreamLatencyMeasurementTest.h"

using std::cerr;
using Poco::URI;
using Poco::TaskManager;
using Poco::Thread;
using Poco::Exception;
using Poco::Util::ServerApplication;
using Poco::Util::OptionSet;

class MJPEGPerformance : public ServerApplication {
public:
	MJPEGPerformance()
	{
	}
protected:
	void initialize(Application& self)
	{
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);
	}

	int main(const std::vector<std::string>& args)
	{
		TaskManager tm;

		try {
			StreamLatencyMeasurementTest* slm = new StreamLatencyMeasurementTest(URI("http://127.0.0.1:4711/videostream"));
			tm.start(slm);
		}
		catch (Exception& e) {
			cerr << e.displayText() << std::endl;
		}

		waitForTerminationRequest();
		tm.cancelAll();
		tm.joinAll();

		return Application::EXIT_OK;
	}
};

POCO_SERVER_MAIN(MJPEGPerformance)