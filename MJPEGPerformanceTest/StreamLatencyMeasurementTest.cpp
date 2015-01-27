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

#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
	|| defined(WIN64) || defined(_WIN64) || defined(__WIN64__)

#include <windows.h>
bool _qpcInited = false;
double PCFreq = 0.0;
__int64 CounterStart = 0;
void InitCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		std::cout << "QueryPerformanceFrequency failed!\n";
	}
	PCFreq = double(li.QuadPart) / 1000.0f;
	_qpcInited = true;
}
double CLOCK()
{
	if (!_qpcInited) InitCounter();
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart) / PCFreq;
}

#endif

double _avgdur = 0;
double _fpsstart = 0;
double _avgfps = 0;
double _fps1sec = 0;

double avgdur(double newdur)
{
	_avgdur = 0.98*_avgdur + 0.02*newdur;
	return _avgdur;
}

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

		double start = 0.0;
		double end = 0.0;
		int frameno = 0;
		long data = 0;
		Stopwatch sw;

		sw.start();
		while (sw.elapsedSeconds() < 60) {
			double start = CLOCK();

			int size = FindLength();
			if (size == -1) {
				cerr << endl << "could not find length" << endl;
				break;
			}

			MoveToStreamStart();

			GetBytes(size);
			data += size;

			double dur = CLOCK() - start;
			printf("frame: %06.2f ms; avg/frame %06.2f ms; frame#: %04u; bytes: %08u\r", dur, avgdur(dur), frameno++, data);
		}

		printf("\n\r##################################\n\r");
		printf("time: %u sec; avg/frame %.2f ms; frames: %4u; data: %d mb\r", sw.elapsedSeconds(), _avgdur, --frameno, data >> 20);

		socket.close();
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

bool StreamLatencyMeasurementTest::GetBytes(int bytesToRead) {
	char* buff = new char[bytesToRead];
	int receivedBytes = 0;
	int temp;
	try {
		while (bytesToRead > receivedBytes) {
			temp = socket.receiveBytes(buff + receivedBytes, bytesToRead - receivedBytes);
			if (temp == 0) std::cout << "empty frame!" << std::endl;
			receivedBytes += temp;
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