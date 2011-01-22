#include "ieee/drivers/LaserSensor.h"
#include "ieee/drivers/V4LCapture.h"
#include "ieee/shared/Timer.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>

using namespace ieee;
using namespace cv;
using namespace std;

struct LaserConfig : LaserSensor::Config {
	LaserConfig() {
		brmult = -1;
		gmult = 2;
		minval = 30;
		maxpoints = 3;
		lasersep = 15;
		exposure = 5000;

		static const LaserSensor::Calibration calibrations_array[] = {
			{ 3.6926e-4, 9.3072e-03, 5},
			{ 3.6926e-4, 9.3072e-03, 5},
			{ 3.6926e-4, 9.3072e-03, 5}
		};

		calibrations = calibrations_array;
		viewangle = 70.0 / 180 * M_PI;
	};
};

typedef Mat (*DisplayFunc)(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig);
static Mat displayRawFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig);
static Mat displayGreenFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig);
static Mat displayReadingFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig);

static const DisplayFunc displayfuncs[] = {
	displayRawFrame,
	displayGreenFrame,
	displayReadingFrame,
	NULL
};

static void putPix(Mat &mat, int row, int col, int color);

int main(int argc, char **argv) {
	namedWindow("frame");

	LaserConfig laserconfig;
	LaserSensor::Debug laserdebug;
	laserconfig.debug = &laserdebug;

	const auto_ptr<LaserSensor> lasersensorptr(LaserSensor::createAndHandleExposureFailure(laserconfig));
	LaserSensor &lasersensor = *lasersensorptr;

	bool showtiming = false;
	int display = 0;

	while (true) {
		Timer tim;
		Mat frame = displayfuncs[display](lasersensor, laserdebug, laserconfig);
		if (showtiming)
			cout << "Time: " << tim.getMilliseconds() << " ms" << endl;
		imshow("frame", frame);

		int key;
		while ((key=waitKey(10)) >= 0) {
			char chkey = (char)key;
			if (chkey == 'u') {
				laserconfig.minval += 5;
				cout << "minval " << laserconfig.minval << endl;
			} else if (chkey == 'd') {
				laserconfig.minval -= 5;
				cout << "minval " << laserconfig.minval << endl;
			} else if (chkey == 'v') {
				cout << "middle values ";
				for (int laser=0; laser<laserdebug.rawreadings.size(); laser++) {
					const vector<int> &vals = laserdebug.rawreadings[laser];
					cout << vals[vals.size()/2] << " ";
				}
				cout << endl;
			} else if (chkey == 'e') {
				laserconfig.exposure += 100;
				cout << "exposure " << laserconfig.exposure << endl;
				lasersensor.setExposure(laserconfig.exposure);
			} else if (chkey == 'r') {
				laserconfig.exposure -= 100;
				cout << "exposure " << laserconfig.exposure << endl;
				lasersensor.setExposure(laserconfig.exposure);
			} else if (chkey == 't') {
				showtiming = !showtiming;
			} else if (chkey == 'i') {
				if (displayfuncs[++display] == NULL)
					display = 0;
			} else {
				return 0;
			}
		}
	}
}

static void putPix(Mat &mat, int row, int col, int color) {
	for (int dr=0; dr<=0; dr++) {
		for (int dc=0; dc<=0; dc++) {
			const int r = row+dr;
			const int c = col+dc;

			if (r < 0 || c < 0 || r >= mat.rows || c >= mat.cols)
				continue;

			uchar *base = &mat.data[3*(r*mat.cols + c)];
			base[0] = 0;
			base[1] = 0;
			base[2] = 0;
			base[color] = 255;
		}
	}
}

static Mat displayRawFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig) {
	LaserSensor::RawReadings readings = laser.captureRawReadings();

	Mat rawframe = laserdebug.rawframe;
	for (int laser=0; laser<readings.size(); laser++) {
		const vector<int> &vals = readings[laser];
		for (int col=0; col<vals.size(); col++) {
			const int row = vals[col];
			if (row == -1)
				continue;

			putPix(rawframe, row, col, laser);
		}
	}

	return rawframe;
}

static Mat displayGreenFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig) {
	laser.captureRawReadings();

	return laserdebug.greenframe;
}

static Mat displayReadingFrame(LaserSensor &laser, const LaserSensor::Debug &laserdebug, const LaserSensor::Config &laserconfig) {
	LaserSensor::Readings readings = laser.captureReadings();

	Mat image(200, 200, CV_8UC3, Scalar(0, 0, 0));

	for (int laser=0; laser<laserconfig.maxpoints; laser++) {
		const LaserSensor::DistAngleVec &distangles = readings[laser];

		for (LaserSensor::DistAngleVec::const_iterator i = distangles.begin(); i != distangles.end(); ++i) {
			Coord c = i->toCoord(M_PI/2);
			float x = image.cols/2 + c.x/100*image.cols;
			float y = (image.rows-1) - c.y/100*image.rows;

			putPix(image, (int)y, (int)x, laser);
		}
	}

	line(image, Point(image.cols/2, image.rows-1), Point(image.rows*tan(laserconfig.viewangle/2) + image.cols/2, 0), Scalar(255, 255, 255));
	line(image, Point(image.cols/2, image.rows-1), Point(image.rows*tan(-laserconfig.viewangle/2) + image.cols/2, 0), Scalar(255, 255, 255));

	return image;
}


