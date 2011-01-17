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
	};
};

static void putPix(Mat &mat, int row, int col, int color);

int main(int argc, char **argv) {
	namedWindow("frame");

	LaserConfig laserconfig;
	LaserSensor::Debug laserdebug;
	laserconfig.debug = &laserdebug;
	LaserSensor lasersensor(laserconfig);

	bool showtiming = false;
	bool showgreen = false;

	while (true) {
		Timer tim;
		LaserSensor::RawReadings readings = lasersensor.captureRawReadings();
		if (showtiming)
			cout << "getReadings(): " << tim.getMilliseconds() << " ms" << endl;

		Mat &rawframe = laserdebug.rawframe;

		for (int laser=0; laser<readings.size(); laser++) {
			const vector<int> &vals = readings[laser];
			for (int col=0; col<vals.size(); col++) {
				const int row = vals[col];
				if (row == -1)
					continue;

				putPix(rawframe, row, col, laser);
			}
		}

		if (showgreen)
			imshow("frame", laserdebug.greenframe);
		else
			imshow("frame", rawframe);

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
				for (int laser=0; laser<readings.size(); laser++) {
					const vector<int> &vals = readings[laser];
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
				showgreen = !showgreen;
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

