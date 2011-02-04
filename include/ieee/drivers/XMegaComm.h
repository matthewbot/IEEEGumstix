#ifndef XMEGACOMM_H
#define XMEGACOMM_H

#include "ieee/drivers/SerialPort.h"
#include "ieee/drivers/SerialPackets.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>
#include <string>
#include <stdint.h>

namespace ieee {
	class XMegaComm {
		public:
			XMegaComm(const std::string &device, boost::posix_time::time_duration timeout=boost::posix_time::millisec(200), int recvbuflen=128);

			inline AVRPacket &getAVRPacket() { return avrpacket; }
			inline const AVRPacket &getAVRPacket() const { return avrpacket; }
			inline GumstixPacket &getGumstixPacket() { return gumstixpacket; }
			inline const GumstixPacket &getGumstixPacket() const { return gumstixpacket; }

			bool ok() const;

			bool sync();
			bool syncIn();
			void syncOut();

		private:
			bool checkRecvbufPacket(int pos);
			static uint8_t checksum(const uint8_t *data, int len);

			SerialPort port;
			boost::posix_time::time_duration timeout;

			boost::posix_time::ptime lastpacket;
			AVRPacket avrpacket;
			GumstixPacket gumstixpacket;

			std::vector<uint8_t> recvbuf;
			int recvbufpos;
	};
}

#endif
