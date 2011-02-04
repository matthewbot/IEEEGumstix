#ifndef SERIALPORTBUF_H
#define SERIALPORTBUF_H

#include "ieee/drivers/SerialPort.h"
#include <vector>

namespace ieee {
	class SerialPortBuf {
		public:
			SerialPortBuf(int len);

			inline int getCount() const { return count; }
			inline const uint8_t *getData() const { return &data[0]; }
			inline const uint8_t &operator[](int pos) const { return data[pos]; }

			void fill(SerialPort &port);
			void drop(int amt);

		private:
			std::vector<uint8_t> data;
			int count;
	};
}

#endif
