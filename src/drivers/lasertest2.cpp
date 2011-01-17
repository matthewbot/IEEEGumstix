#include "ieee/drivers/LaserSensor.h"
#include "ieee/drivers/V4LCapture.h"
#include "ieee/shared/Timer.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include <cmath>

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
		viewangle = M_PI/3;
	};
};

static void putPix(Mat &mat, int r, int c, int color);

int main(int argc, char **argv) {
	namedWindow("frame");

	LaserConfig laserconfig;
	LaserSensor::Debug laserdebug;
	laserconfig.debug = &laserdebug;
	LaserSensor lasersensor(laserconfig);

	while (true) {
		LaserSensor::Readings readings = lasersensor.captureReadings();

		Mat image(200, 200, CV_8UC3, Scalar(0, 0, 0));

		for (int laser=0; laser<laserconfig.maxpoints; laser++) {
			const LaserSensor::DistAngleVec &distangles = readings[laser];

			for (LaserSensor::DistAngleVec::const_iterator i = distangles.begin(); i != distangles.end(); ++i) {
				float x = image.cols/2 + sin(i->angle)*i->dist/100*image.cols;
				float y = (image.rows-1) - cos(i->angle)*i->dist/100*image.rows;

				putPix(image, (int)y, (int)x, laser);
			}
		}

		line(image, Point(image.cols/2, image.rows-1), Point(image.rows*tan(laserconfig.viewangle/2) + image.cols/2, 0), Scalar(255, 255, 255));
		line(image, Point(image.cols/2, image.rows-1), Point(image.rows*tan(-laserconfig.viewangle/2) + image.cols/2, 0), Scalar(255, 255, 255));

		imshow("frame", image);

		if(waitKey(10) >= 0) break;
	}
}

static void putPix(Mat &mat, int r, int c, int color) {
	if (r < 0 || c < 0 || r >= mat.rows || c >= mat.cols)
		return;

	uchar *base = &mat.data[3*(r*mat.cols + c)];
	base[0] = 0;
	base[1] = 0;
	base[2] = 0;
	base[color] = 255;
}

