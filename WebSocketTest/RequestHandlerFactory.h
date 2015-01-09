#pragma once
#include <iostream>
#include <string>

#include <Poco\Net\HTTPRequestHandlerFactory.h>
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPRequestHandler.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Exception.h>
#include <Poco\Net\NetException.h>

using std::cerr;
using std::cout;
using std::endl;

using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;

class WebSocketRequestHandler : public HTTPRequestHandler
{
public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		try
		{
			WebSocket ws(request, response);
			cout << "WebSocket connection established.";
			char buffer[1024];
			int flags = WebSocket::FRAME_TEXT;
			int n;
			bool initReceived = false;

			/* Protocol (based on JSON object)
			* {
			* 	 to: 'test',
			* 	 cmd: 'echo',
			*   params: {
			*	     toUpper: true
			*	 },
			*	 data: 'testdata'
			* }
			*/
			//std::string msg = "{\"cmd\":\"echo\", \"to\":\"cpp\", \"params\":{ \"toUpper\":\"true\"}, \"data\":\"testdata\"}";

			do
			{
				n = ws.receiveFrame(buffer, sizeof(buffer), flags);
				cout << Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)) << endl;

				std::string msg = std::string(buffer, n);

				cout << msg << endl;

				if (!initReceived) {
					std::string msg = "{\"cmd\":\"start\"}";

					n = ws.sendFrame(msg.data(), msg.size(), flags);
					cout << msg << endl;
					cout << Poco::format("Frame sent (length=%d, flags=0x%x).", n, unsigned(flags)) << endl;
				}
			} while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);

			cout << "WebSocket connection closed.";
		}
		catch (WebSocketException& exc)
		{
			cerr << exc.displayText() << endl;

			switch (exc.code())
			{
			case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
				// fallthrough
			case WebSocket::WS_ERR_NO_HANDSHAKE:
			case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}
	}
};

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		cout << "Request from " << request.clientAddress().toString() << ": " << request.getMethod()
			<< " " << request.getURI() << " " << request.getVersion();

		for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
		{
			cout << it->first << ": " << it->second << endl;
		}

		return new WebSocketRequestHandler;
	}
};
