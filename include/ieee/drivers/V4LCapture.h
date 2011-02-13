#ifndef V4LCAPTURE_H
#define V4LCAPTURE_H

#include <string>
#include <utility>
#include "ieee/drivers/Image.h"

namespace ieee {
	class V4LCapture {
		public:
			V4LCapture(int width, int height, const std::string &filename="", int exposure=-1, int bufs=2);
			~V4LCapture();

			inline int getWidth() { return width; }
			inline int getHeight() { return height; }

			void readFrame(Image &mat);

			void setAutoExposure(bool on=true);
			void setExposure(int exposure);

		private:
			std::pair<bool, std::string> scanDevices();
			std::pair<bool, std::string> openDevice(const std::string &filename);

			bool setControl(int id, int val);

			int fd;
			int width, height;
	};
}

#endif
