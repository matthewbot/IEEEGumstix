#include "ieee/drivers/avr/AVRComm.h"
#include <cstring>

using namespace ieee;
using namespace boost::posix_time;
using namespace std;

AVRComm::AVRComm(const string &device, time_duration timeout, int recvbuflen)
: port(device), timeout(timeout), recvbuf(recvbuflen) { }

bool AVRComm::ok() const {
	return microsec_clock::local_time() - lastpacket < timeout;
}

bool AVRComm::syncIn(AVRPacket &avr) {
	recvbuf.fill(port); // pull new data into the receive buffer

	int maxpos = recvbuf.getCount() - sizeof(AVRPacket); // determine the farthest back a packet could be based on size
	if (maxpos < 0) // if we don't have enough bytes for a new packet
		return false; // then we've already failed

	int pos;
	for (pos=maxpos; pos>=0; pos--) { // start from there and work your way to the front of the buffer looking for a packet
		if (checkRecvbufPacket(pos))
			break;
	}

	if (pos < 0) { // didn't find any packets
		recvbuf.drop(maxpos+1); // drop enough bytes so that we're one byte short of a packet
		return false;
	}

	memcpy(&avr, &recvbuf[pos], sizeof(AVRPacket)); // otherwise, we got a packet, so copy it into avrpacket
	recvbuf.drop(pos+sizeof(AVRPacket)); // then drop all the bytes that made up the packet, and all bytes before the packet

	lastpacket = microsec_clock::local_time(); // record the current time
	return true;
}

void AVRComm::syncOut(GumstixPacket &gumstix) {
	gumstix.header = 0x1EEE;
	gumstix.protoversion = 2;
	uint8_t *data = reinterpret_cast<uint8_t *>(&gumstix);
	gumstix.checksum = checksum(data, sizeof(GumstixPacket)-1);
	port.write(data, sizeof(GumstixPacket));
}

bool AVRComm::checkRecvbufPacket(int pos) {
	const AVRPacket *packet = reinterpret_cast<const AVRPacket *>(&recvbuf[pos]);
	if (packet->header != 0x1EEE)
		return false;

	if (packet->protoversion != 2)
		return false;

	if (packet->checksum != checksum(&recvbuf[pos], sizeof(AVRPacket)-1)) // -1 to not include the checksum byte in the checksum
		return false;

	return true;
}

uint8_t AVRComm::checksum(const uint8_t *data, int length) {
	int sum=0;
	while (length--)
		sum += *data++;
	return (uint8_t)sum;
}

