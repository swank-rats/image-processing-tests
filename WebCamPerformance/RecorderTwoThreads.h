#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <sys/timeb.h>

#include <Poco\Thread.h>
#include <Poco\RunnableAdapter.h>
#include <Poco\Activity.h>
#include <Poco\Logger.h>
#include <Poco\RWLock.h>
#include <Poco\Stopwatch.h>
#include <memory>

using std::cout;
using namespace cv;

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Logger;
using Poco::Stopwatch;
using Poco::RunnableAdapter;
using Poco::Thread;

class RecorderTwoThreads {
public:
	RecorderTwoThreads();

	void start();
	void process();
	void record();
private:
	bool isNew;
	Mat lastImage;
	VideoCapture capture;
	Activity<RecorderTwoThreads> recordingActivity;
	Activity<RecorderTwoThreads> processingActivity;

};


RecorderTwoThreads::RecorderTwoThreads() :
recordingActivity(this, &RecorderTwoThreads::record),
processingActivity(this, &RecorderTwoThreads::process) { }

void RecorderTwoThreads::start() {
	recordingActivity.start();
	processingActivity.start();
}

void RecorderTwoThreads::process() {
	Stopwatch sw;
	while (1) {
		if (isNew) {
			sw.restart();
			capture.retrieve(lastImage);

			if (!lastImage.empty()) {
				isNew = false;
			}

			sw.stop();
			printf("Processing frame: %f ms\n", sw.elapsed() * 0.001);
		}
	}
}

void RecorderTwoThreads::record() {
	Stopwatch sw;
	cv::Mat frame;

	capture.open(CV_CAP_ANY);
	//camera settings
	capture.set(CV_CAP_PROP_FPS, 30);
	//Possible resolutions : 1280x720, 640x480; 440x330
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (1) {
		if (!capture.isOpened()) {
			break;
		}

		sw.restart();

		if (capture.grab()) {
			isNew = true;
		}

		sw.stop();
		printf("Capture frame: %f ms\n", sw.elapsed() * 0.001);

		if (waitKey(1) == 27)
			exit(0);
	}
}