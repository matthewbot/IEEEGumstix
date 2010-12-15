#ifndef V4LCAPTURE_H
#define V4LCAPTURE_H

#include <string>
#include <opencv/cv.h>

namespace ieee {
	class V4LCapture {
		public:
			V4LCapture(const std::string &filename, int width, int height);
			~V4LCapture();
			
			inline int getWidth() { return width; }
			inline int getHeight() { return height; }
			
			void readFrame(cv::Mat &mat);
		
		private:
			int fd;
			int width, height;
	};
}

#endif