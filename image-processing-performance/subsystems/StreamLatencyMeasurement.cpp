#include <iostream>

#include <Poco\Stopwatch.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>

#include "StreamLatencyMeasurement.h"

StreamLatencyMeasurement::StreamLatencyMeasurement(Poco::URI uri) 
	: Task("StreamLatencyMeasurement"), session(uri.getHost(), uri.getPort()), path(uri.getPathAndQuery())
{
}


void StreamLatencyMeasurement::runTask()
{
	try {
		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
		session.sendRequest(req);

		Poco::Stopwatch sw;

		while (!isCancelled()) {
			Poco::Net::HTTPResponse res;
			sw.restart();
			std::istream& is = session.receiveResponse(res);
			sw.stop();
			// Print to standard output
			std::cout << sw.elapsedSeconds() << '\r';
		}

		session.abort();
	}
	catch (Poco::Exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cerr << "Message: " << e.message() << std::endl;
		std::cerr << e.displayText() << std::endl;
	}
}
