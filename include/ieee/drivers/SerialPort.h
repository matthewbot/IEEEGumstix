#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace ieee {
	class SerialPort : boost::noncopyable {
		public:
			SerialPort(const std::string &device);
			~SerialPort();

			size_t read(uint8_t *buf, size_t len);
			void write(const uint8_t *buf, int len);

		private:
			void throwError(const std::string &msg);

			int portfd;
	};
}

#endif
