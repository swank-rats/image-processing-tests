#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <sys/timeb.h>
using namespace cv;

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

void process(Mat& frame)
{
	Sleep(3);
}
int main(int argc, char** argv)
{
	int frameno = 0;
	cv::Mat frame;
	cv::VideoCapture cap(0);

	for (;;)
	{
		//cap>>frame;
		double start = CLOCK();
		process(frame);
		double dur = CLOCK() - start;
		printf("avg time per frame %f ms. frameno = %d\n", avgdur(dur), frameno++);

		if (waitKey(1) == 27)
			exit(0);
	}
	return 0;
}