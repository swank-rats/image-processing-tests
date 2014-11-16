#include <iostream>
#include <iomanip>

#include <Poco\Stopwatch.h>
#include <Poco\Stopwatch.h>


#include "StreamLatencyMeasurementTest.h"

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

StreamLatencyMeasurementTest::StreamLatencyMeasurementTest(URI uri)
	: uri(uri), Task("StreamLatencyMeasurementTest"), socketAddr(uri.getHost(), uri.getPort()), socket(), stream(socket)
{
}

void StreamLatencyMeasurementTest::runTask()
{

	try {
		socket.connect(socketAddr);

		string request = "GET /videostream HTTP/1.1\r\nHost: 127.0.0.1:4711\r\nConnection: keep - alive\r\nAccept : image / webp, */*;q=0.8\r\n\r\n";

		SendFrame(request);

		Stopwatch sw;
		double timeBetween;

		while (!isCancelled()) {
			sw.restart();
			int size = FindLength();
			if (size == -1) {
				cerr << endl << "could not find length" << endl;
				break;
			}

			MoveToStreamStart();

			char* buff = new char[size];
			sw.stop();
			timeBetween = sw.elapsed() * 0.001;
			sw.start();
			GetBytes(buff, size);
			sw.stop();
			cout << "transport of " << size << " bytes: " << std::setw(8) << sw.elapsed() * 0.001 << " ms; time between: " << std::setw(7) << timeBetween << " ms\r";
		}
	}
	catch (Exception& e) {
		cerr << endl;
		cerr << "Exception: " << e.what() << endl;
		cerr << "Message: " << e.message() << endl;
		cerr << e.displayText() << endl;
	}
}

void StreamLatencyMeasurementTest::MoveToStreamStart() {
	while (1) {
		string response;

		GetFrame(response, '\n');
		std::size_t start = response.find(jpeg);

		if (start != 0 && start != string::npos) {
			return;
		}
	}
}

int StreamLatencyMeasurementTest::FindLength() {
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

bool StreamLatencyMeasurementTest::GetBytes(char bytes[], int bytesToRead) {
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


bool StreamLatencyMeasurementTest::GetFrame(std::string& frame, char delimiter) {
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

bool StreamLatencyMeasurementTest::SendFrame(const std::string& frame) {
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