#include "ieeepath/vision/V4LCapture.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace ieeepath;
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	V4LCapture cap("/dev/video1");
	
	V4LCapture::Frame capframe;
	
	namedWindow("frame");
	
	while (true) {
		cap.readFrame(capframe);
		Mat frame(240, 320, CV_8UC3, capframe);
		imshow("frame", frame);
		
		if(waitKey(30) >= 0) break;
	}
}

