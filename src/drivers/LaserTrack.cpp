#include "ieee/drivers/LaserTrack.h"
#include <algorithm>
#include <stdint.h>

using namespace ieee;
using namespace std;

LaserTrack::LaserTrack(const Config &config, const Image &frame)
: config(config),
  frame(frame),
  linevec(config.maxpoints) {
	for (LineVec::iterator i = linevec.begin(); i != linevec.end(); ++i) {
		i->resize(frame.getCols());
	}

	for (int col=0; col<frame.getCols(); col+=2) {
		scanCol(col);
		for (int i=0; i<config.maxpoints; i++)
			linevec[i][col+1] = linevec[i][col];
	}
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

	const int realminval = (config.minval << 8)-197248;

	for (int row=0; row<frame.getRows(); row++) {
		const int val = pixVal(frame.getPixel(row, col)); // get the value of the current pixel
		if (val < realminval) // if it doesn't exceed the threshold
			continue; // we skip it

		int bestlhrow=row;
		int bestlhval=val;
		for (int lh=1; lh < config.lasersep; lh++) { // for each lookahead pixel
			if (lh+row >= frame.getRows()) // don't go past the end of the frame
				break;

			int lhval = pixVal(frame.getPixel(row, col)); // get a new value for the lookahead pixel
			if (lhval > bestlhval) { // if its greater than the best lookahead val
				bestlhval = lhval; // its the new best lookahead val
				bestlhrow = lh+row;
			}
		}
		row = bestlhrow + config.lasersep; // advance our current row by the laser seperation since we already got these pixels

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

int LaserTrack::pixVal(const uint8_t *pix) const {
	return -716*pix[1] - 825*pix[3];
}

Image LaserTrack::generateGreenChannel() const {
	Image out(frame.getRows(), frame.getCols(), Image::GRAYSCALE);

	for (int row=0; row<frame.getRows(); row++) {
		for (int col=0; col<frame.getCols(); col++) {
			int pixval = (pixVal(frame.getPixel(row, col)) + 197248) >> 8;
			if (pixval < 0)
				pixval = 0;
			else if (pixval > 255)
				pixval = 255;
			*out.getPixel(row, col) = (uint8_t)pixval;
		}
	}

	return out;
}

