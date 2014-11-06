#include <iostream>

#include <Poco\Stopwatch.h>
#include <Poco\Stopwatch.h>


#include "StreamLatencyMeasurement.h"

using std::cerr;
using std::cout;
using std::endl;
using Poco::Stopwatch;
using Poco::Exception;

string boundary = "--VIDEOSTREAM\r\n";
string contentLength = "Content-Length: ";
string length = "Length";
string jpeg = "jpeg\r\n";
string lineend = "\r\n";

StreamLatencyMeasurement::StreamLatencyMeasurement(URI uri)
	: uri(uri), Task("StreamLatencyMeasurement"), socketAddr(uri.getHost(), uri.getPort()), socket(), stream(socket)
{
}

void StreamLatencyMeasurement::runTask()
{

	try {
		socket.connect(socketAddr);

		string request = "GET /videostream HTTP/1.1\r\nHost: 127.0.0.1:4711\r\nConnection: keep - alive\r\nAccept : image / webp, */*;q=0.8\r\n\r\n";

		SendFrame(request);

		Stopwatch sw;

		while (!isCancelled()) {
			int size = FindLength();
			if (size == -1) {
				cerr << endl << "could not find length" << endl;
				break;
			}

			MoveToStreamStart();

			char* buff = new char[size];
			
			sw.start();
			GetBytes(buff, size);
			sw.stop();
			cout << "latency (ms): " << sw.elapsed() * 0.001 << "\r";
			sw.reset();
		}
	}
	catch (Exception& e) {
		cerr << endl;
		cerr << "Exception: " << e.what() << endl;
		cerr << "Message: " << e.message() << endl;
		cerr << e.displayText() << endl;
	}
}

void StreamLatencyMeasurement::MoveToStreamStart() {
	while (1) {
		string response;

		GetFrame(response, '\n');
		std::size_t start = response.find(jpeg);

		if (start != 0 && start != string::npos) {
			return;
		}
	}
}

int StreamLatencyMeasurement::FindLength() {
	bool foundLength = false;
	int size = -1;

	while (!foundLength) {
		string response;

		GetFrame(response, '\n');
		std::size_t start = response.find(length);

		if (start != 0 && start != string::npos) {
			std::size_t end = response.find_last_of(lineend);

			string strSize = response.substr(contentLength.length(), 5);
			size = stoi(strSize);
			foundLength = true;
		}
	}

	return size;
}

bool StreamLatencyMeasurement::GetBytes(char bytes[], int bytesToRead) {
	int tmp = 0;
	try {
		while (bytesToRead > tmp) {
			tmp += socket.receiveBytes(bytes + tmp, bytesToRead - tmp);
		}
	}
	catch (Exception error) {
		std::cout << "recv failed (Error: " << error.displayText() << ')' << std::endl;
		return false;
	}
	return true;
}


bool StreamLatencyMeasurement::GetFrame(std::string& frame, char delimiter) {
	char c;
	try {
		do {
			socket.receiveBytes(&c, 1);
			frame.append(1, c);
		} while (delimiter != c);
	}
	catch (Exception e) {
		cerr << e.displayText() << endl;
		return false;
	}
	return true;
}

bool StreamLatencyMeasurement::SendFrame(const std::string& frame) {
	int tmp = 0;
	int bytesToWrite = frame.length();
	const char* bytes = frame.c_str();

	try {
		while (bytesToWrite > tmp) {
			tmp += socket.sendBytes(bytes + tmp, bytesToWrite - tmp);
		}
	}
	catch (Exception e) {
		cerr << e.displayText() << endl;
		return false;
	}

	return true;
}