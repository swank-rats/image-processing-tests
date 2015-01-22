#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <Poco\Thread.h>
#include <Poco\Activity.h>
#include <Poco\Logger.h>
#include <Poco\RWLock.h>
#include <Poco\Stopwatch.h>

#include <memory>

using std::cout;

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Logger;
using Poco::Stopwatch;

class Recorder {
public:
	Recorder();

	void start();
private:
	void recording();

	Mat lastImage;
	VideoCapture capture;
	Activity<Recorder> recordingActivity;
};

Recorder::Recorder() : recordingActivity(this, &Recorder::recording) {
	capture.open(CV_CAP_ANY);
	//camera settings
	capture.set(CV_CAP_PROP_FPS, 30);
	//Possible resolutions : 1280x720, 640x480; 440x330
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

void Recorder::start() {
	recordingActivity.start();
}

void Recorder::recording() {
	Logger& logger = Logger::get("WebcamService");
	Stopwatch sw;
	cv::Mat frame;

	while (!recordingActivity.isStopped()) {
		if (!capture.isOpened()) {
			logger.error("Lost connection to webcam!");
			break;
		}

		sw.restart();

		//Create image frames from capture
		capture >> frame;

		if (!frame.empty()) {
			//Clone image
			lastImage = frame;
		}
		else {
			logger.warning("Captured empty webcam frame!");
		}

		sw.stop();
		cout << sw.elapsed() * 0.001 << " ms\n\r";
	}
}