#ifndef XMEGACOMM_H
#define XMEGACOMM_H

#include "ieee/drivers/SerialPort.h"
#include "ieee/drivers/SerialPackets.h"
#include "ieee/drivers/SerialPortBuf.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>
#include <string>
#include <stdint.h>

namespace ieee {
	class XMegaComm {
		public:
			XMegaComm(const std::string &device="", boost::posix_time::time_duration timeout=boost::posix_time::millisec(200), int recvbuflen=128);

			bool ok() const;

			bool sync(AVRPacket &avr, GumstixPacket &gumstix);
			bool syncIn(AVRPacket &avr);
			void syncOut(GumstixPacket &gumstix);

		private:
			bool checkRecvbufPacket(int pos);
			static uint8_t checksum(const uint8_t *data, int len);

			SerialPort port;
			boost::posix_time::time_duration timeout;

			boost::posix_time::ptime lastpacket;

			SerialPortBuf recvbuf;
	};
}

#endif
