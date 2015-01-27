#include "RecorderActivity.h"
#include "RecorderThread.h"
#include "RecorderTwoThreads.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/core/gpumat.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <sys/timeb.h>

#include <Poco\Thread.h>
#include <Poco\Activity.h>
#include <Poco\Logger.h>
#include <Poco\RWLock.h>
#include <Poco\Stopwatch.h>
#include <memory>

using std::cout;
using namespace cv;
using namespace cv::gpu;

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Logger;
using Poco::Stopwatch;

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

int main(int argc, char** argv) {
	//Run POCO activity performance
	//RecorderActivity rec;
	//rec.start();
	//while (1);

	//Run POCO thread performance
	RecorderThread rec;
	Poco::ThreadPool::defaultPool().start(rec);
	Poco::ThreadPool::defaultPool().joinAll();
	cv::gpu::GpuMat x, y;

//Run POCO threads processing and capturing
	//RecorderTwoThreads rec;
	//rec.start();
	//while (1);

	//run in main
	//int frameno = 0;
	//cv::Mat frame;
	//cv::Mat lastImage;
	//cv::VideoCapture cap;
	//cap.open(CV_CAP_DSHOW + 0);
	////camera settings
	//cap.set(CV_CAP_PROP_FPS, 30);
	////Possible resolutions : 1280x720, 640x480; 440x330
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	//for (;;)
	//{
	//	double start = CLOCK();
	//	cap >> frame;
	//	lastImage = frame; //Clone

	//	process(frame);

	//	double dur = CLOCK() - start;
	//	printf("time this frame: %f ms. avg time per frame %f ms. frameno = %d\r", dur, avgdur(dur), frameno++);

	//	if (waitKey(1) == 27)
	//		exit(0);
	//}

	return 0;
}