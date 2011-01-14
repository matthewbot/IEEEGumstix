#include "ieee/drivers/LaserSensor.h"
#include "ieee/drivers/LaserSensorDebug.h"
#include <opencv/cv.h>
#include <vector>

using namespace ieee;
using namespace cv;
using namespace std;

LaserSensor::LaserSensor(V4LCapture &cap, const Config &config)
: cap(cap), config(config) { }

vector<int> LaserSensor::getReadings(LaserSensorDebug *debug) {
	Mat frame;
	cap.readFrame(frame);

	vector<int> out(frame.cols);
	for (int col=0; col<frame.cols; col++) { // for each column
		out[col] = scanCol(frame, col);
	}

	if (debug) {
		debug->rawframe = frame;
		debug->greenframe.create(frame.rows, frame.cols, CV_8UC1);

		for (int row=0; row<frame.rows; row++) {
			for (int col=0; col<frame.cols; col++) {
				debug->greenframe.data[row*frame.cols + col] = saturate_cast<uchar>(pixVal(&frame.data[3*(row*frame.cols + col)]));
			}
		}
	}

	return out;
}

int LaserSensor::scanCol(const Mat &frame, const int col) const {
	int row = frame.rows;

	while (true) {
		while (--row >= 0) {
			if (checkPix(&frame.data[3*(row*frame.cols + col)]))
				break;
		}
		if (row < 0)
			return -1;
		const int segstart = row;

		while (--row >= 0) {
			if (!checkPix(&frame.data[3*(row*frame.cols + col)]))
				break;
		}
		const int segend = row;

		const int segheight = segstart - segend;

		if (segheight < config.minsegheight)
			continue;

		return segend + segheight/2;
	}
}

int LaserSensor::pixVal(const uchar *pix) const {
	int br = (int)pix[0] + pix[2];
	int g = pix[1];
	return config.brmult*br + config.gmult*g;
}

