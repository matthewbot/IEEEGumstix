#include "ieee/drivers/LaserTrack.h"
#include <opencv/cv.h>
#include <utility>

using namespace ieee;
using namespace cv;
using namespace std;

LaserTrack::LaserTrack(const Config &config, const Mat &frame)
: config(config),
  frame(frame),
  linevec(config.maxpoints) {
	for (LineVec::iterator i = linevec.begin(); i != linevec.end(); ++i) {
		i->resize(frame.cols);
	}

	for (int col=0; col<frame.cols; col++)
		scanCol(col);
}

namespace {
	struct Entry {
		Entry() { }
		Entry(int row, int val) : row(row), val(val) { }

		inline bool operator==(const Entry &other) const {
			return row == other.row;
		}
		inline bool operator<(const Entry &other) const {
			return row < other.row;
		}

		int row;
		int val;
	};
}

void LaserTrack::scanCol(int col) {
	vector<Entry> greatestrows;

	for (int row=0; row<frame.rows; row++) {
		const int val = pixVal(&frame.data[3*(row*frame.cols + col)]); // get the value of the current pixel
		if (val < config.minval) // if it doesn't exceed the threshold
			continue; // we skip it

		int bestlhrow=row;
		int bestlhval=val;
		for (int lh=1; lh < config.lasersep; lh++) { // for each lookahead pixel
			if (lh+row >= frame.rows) // don't go past the end of the frame
				break;

			int lhval = pixVal(&frame.data[3*((lh+row)*frame.cols + col)]); // get a new value for the lookahead pixel
			if (lhval > bestlhval) { // if its greater than the best lookahead val
				bestlhval = lhval; // its the new best lookahead val
				bestlhrow = lh+row;
			}
		}
		row += config.lasersep; // advance our current row by the laser seperation since we already got these pixels

		vector<Entry>::iterator pos = greatestrows.begin(); // go through each of our greatest entry rows
		for (; pos != greatestrows.end(); ++pos) {
			if (bestlhval > pos->val) // if our best lookahead val is better
				break; // we need to insert it here
		}

		if (pos == greatestrows.end() && greatestrows.size() >= config.maxpoints) // if we didn't fit in the best vector and we already have enough best values
			continue; // skip it

		greatestrows.insert(pos, Entry(bestlhrow, bestlhval)); // otherwise insert our new value

		if (greatestrows.size() > config.maxpoints) // and possibly truncate the vector to keep only our needed values
			greatestrows.resize(config.maxpoints);
	}

	sort(greatestrows.begin(), greatestrows.end());
	for (int i=0; i<greatestrows.size(); i++) {
		linevec[i][col] = greatestrows[i].row;
	}

	for (int i=greatestrows.size(); i<config.maxpoints; i++) {
		linevec[i][col] = -1;
	}
}

int LaserTrack::pixVal(const uchar *pix) const {
	int br = (int)pix[0] + pix[2];
	int g = pix[1];
	return config.brmult*br + config.gmult*g;
}

cv::Mat LaserTrack::generateGreenChannel() const {
	Mat out;
	out.create(frame.rows, frame.cols, CV_8UC1);

	for (int row=0; row<frame.rows; row++) {
		for (int col=0; col<frame.cols; col++) {
			out.data[row*frame.cols + col] = saturate_cast<uchar>(pixVal(&frame.data[3*(row*frame.cols + col)]));
		}
	}

	return out;
}

