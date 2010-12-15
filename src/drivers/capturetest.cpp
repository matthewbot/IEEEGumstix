#include "ieee/drivers/V4LCapture.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace ieee;
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	V4LCapture cap("/dev/video0", 640, 480);
	
	namedWindow("frame");
	
	Mat frame;
	while (true) {
		cap.readFrame(frame);
		imshow("frame", frame);
		
		if(waitKey(30) >= 0) break;
	}
}

