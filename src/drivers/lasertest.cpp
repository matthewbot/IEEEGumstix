#include "ieee/drivers/LaserSensor.h"
#include "ieee/drivers/LaserSensorDebug.h"
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

		minthresh = 30;
	};
};

static void putRedPix(Mat &mat, int row, int col);

int main(int argc, char **argv) {
	LaserConfig laserconfig;

	V4LCapture cap("/dev/video1", 320, 240);
	LaserSensor lasersensor(cap, laserconfig);
	LaserSensorDebug laserdebug;
	bool showtiming=false;
	bool showgreen=false;

	namedWindow("frame");

	while (true) {
		Timer tim;
		LaserSensor::Readings readings = lasersensor.getReadings(&laserdebug);
		if (showtiming)
			cout << "getReadings(): " << tim.getMilliseconds() << " ms" << endl;

		Mat &rawframe = laserdebug.rawframe;
		for (int col=0; col<readings.size(); col++) {
			const int row = readings[col];
			if (row == -1)
				continue;

			putRedPix(rawframe, row, col);
		}

		if (showgreen)
			imshow("frame", laserdebug.greenframe);
		else
			imshow("frame", rawframe);

		int key;
		if ((key=waitKey(10)) >= 0) {
			char chkey = (char)key;
			if (chkey == 'u') {
				laserconfig.minthresh += 5;
				cout << "minthresh " << laserconfig.minthresh << endl;
			} else if (chkey == 'd') {
				laserconfig.minthresh -= 5;
				cout << "minthresh " << laserconfig.minthresh << endl;
			} else if (chkey == 'v') {
				int val = readings[readings.size()/2];
				cout << "middle value " << val << endl;
			} else if (chkey == 't') {
				showtiming = !showtiming;
			} else if (chkey == 'i') {
				showgreen = !showgreen;
			} else {
				break;
			}
		}
	}
}

static void putRedPix(Mat &mat, int row, int col) {
	for (int dr=0; dr<=0; dr++) {
		for (int dc=0; dc<=0; dc++) {
			const int r = row+dr;
			const int c = col+dc;

			if (r < 0 || c < 0 || r >= mat.rows || col >= mat.cols)
				continue;

			uchar *base = &mat.data[3*(r*mat.cols + c)];
			base[0] = base[1] = 0;
			base[2] = 255;
		}
	}
}

