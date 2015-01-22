//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : WebSocket protocol test
//============================================================================
#include <iostream>

#include <Poco\Exception.h>
#include <Poco\Util\ServerApplication.h>
#include <Poco\Util\OptionSet.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Net\SecureServerSocket.h>
#include <Poco\Net\HTTPServerParams.h>

#include "RequestHandlerFactory.h"

using std::cerr;
using Poco::Exception;
using Poco::Util::ServerApplication;
using Poco::Util::OptionSet;
using Poco::Net::HTTPServer;
using Poco::Net::SecureServerSocket;
using Poco::Net::HTTPServerParams;

class WebSocketTest : public ServerApplication {
public:
	WebSocketTest()
	{
		Poco::Net::initializeSSL();
	}

	~WebSocketTest()
	{
		Poco::Net::uninitializeSSL();
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
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
		try {
			unsigned short port = (unsigned short)config().getInt("WebSocketTest.port", 3001);

			// set-up a server socket
			SecureServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		catch (Exception& e) {
			cerr << e.displayText() << std::endl;
		}

		return Application::EXIT_OK;
	}
};

POCO_SERVER_MAIN(WebSocketTest)