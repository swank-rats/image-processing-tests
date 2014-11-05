#pragma once
#include <string.h>
#include <Poco\Uri.h>
#include <Poco\Task.h>
#include <Poco\Net\HTTPClientSession.h>

using std::string;

class StreamLatencyMeasurement : public Poco::Task
{
public:
	StreamLatencyMeasurement(Poco::URI uri);
	void runTask();
private:
	Poco::Net::HTTPClientSession session;
	std::string path;
};

