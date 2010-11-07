#ifndef V4LCAPTURE_H
#define V4LCAPTURE_H

#include <string>
#include <boost/cstdint.hpp>

namespace ieeepath {
	class V4LCapture {
		public:
			V4LCapture(const std::string &filename);
			~V4LCapture();
			
			struct RGBPixel {
				uint8_t b, g, r;
			} __attribute__((__packed__));

			typedef RGBPixel Frame[320*240];
			void readFrame(Frame &frame);
		
		private:
			int fd;
	};
}

#endif
