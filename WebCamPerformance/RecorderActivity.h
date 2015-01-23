#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
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

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Thread;
using Poco::Logger;
using Poco::Stopwatch;

class RecorderActivity {
public:
	RecorderActivity();

	void start();
private:
	void recording();

	Mat lastImage;
	VideoCapture capture;
	Activity<RecorderActivity> recordingActivity;
};

RecorderActivity::RecorderActivity() : recordingActivity(this, &RecorderActivity::recording) {
}

void RecorderActivity::start() {
	recordingActivity.start();
}

void RecorderActivity::recording() {
	Stopwatch sw;
	cv::Mat frame;
	int fps = 15;
	int sleep = 6000 / fps;

	capture.open(CV_CAP_ANY);
	//camera settings
	capture.set(CV_CAP_PROP_FPS, fps);
	//Possible resolutions : 1280x720, 640x480; 440x330
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (!recordingActivity.isStopped()) {
		if (!capture.isOpened()) {
			break;
		}

		sw.restart();

		//Create image frames from capture
		capture >> frame;

		if (!frame.empty()) {
			//Clone image
			lastImage = frame;
		}

		sw.stop();
		printf("Capture frame: %f ms\n", sw.elapsed() * 0.001);

		if (waitKey(1) == 27)
			exit(0);

		Thread::sleep(sleep);
	}
}