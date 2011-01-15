#include "ieee/drivers/V4LCapture.h"
#include "ieee/shared/Timer.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace ieee;
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	V4LCapture cap(320, 240);

	namedWindow("frame");

	Mat frame;
	while (true) {
		Timer tim;

		cap.readFrame(frame);
		cout << "readFrame(): " << tim.getMilliseconds() << " ms" << endl;
		tim.reset();

		imshow("frame", frame);
		cout << "imshow(): " << tim.getMilliseconds() << " ms" << endl;

		if(waitKey(10) >= 0) break;
	}
}

