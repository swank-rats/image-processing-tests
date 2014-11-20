#pragma once
#include <string.h>
#include <Poco\Uri.h>
#include <Poco\Task.h>
#include <Poco\Net\SocketAddress.h>
#include <Poco\Net\StreamSocket.h>
#include <Poco\Net\SocketStream.h>

using std::string;
using Poco::URI;
using Poco::Task;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;

class StreamLatencyMeasurementTest : public Task
{
public:
	StreamLatencyMeasurementTest(URI uri);
	void runTask();
private:
	URI uri;
	SocketAddress socketAddr;
	StreamSocket socket;
	SocketStream stream;

	bool GetFrame(std::string& frame, char delimiter);
	bool SendFrame(const std::string& frame);
	bool GetBytes(int bytesToRead);
	int FindLength();
	void MoveToStreamStart();
};

