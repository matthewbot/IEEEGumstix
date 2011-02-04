#include "ieee/drivers/XMegaComm.h"
#include <cstring>

using namespace ieee;
using namespace boost::posix_time;
using namespace std;

XMegaComm::XMegaComm(const string &device, time_duration timeout, int recvbuflen)
: port(device), timeout(timeout), recvbuf(recvbuflen) {
	memset(&avrpacket, 0, sizeof(avrpacket));
	memset(&gumstixpacket, 0, sizeof(gumstixpacket));
}

bool XMegaComm::ok() const {
	return microsec_clock::local_time() - lastpacket < timeout;
}

bool XMegaComm::sync() {
	syncOut();
	return syncIn();
}

bool XMegaComm::syncIn() {
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

	memcpy(&avrpacket, &recvbuf[pos], sizeof(AVRPacket)); // otherwise, we got a packet, so copy it into avrpacket
	recvbuf.drop(maxpos+sizeof(AVRPacket)); // then drop all the bytes that made up the packet, and all bytes before the packet

	lastpacket = microsec_clock::local_time(); // record the current time
	return true;
}

void XMegaComm::syncOut() {
	port.write(reinterpret_cast<uint8_t *>(&gumstixpacket), sizeof(gumstixpacket));
}

bool XMegaComm::checkRecvbufPacket(int pos) {
	const AVRPacket *packet = reinterpret_cast<const AVRPacket *>(recvbuf[pos]);
	if (packet->header != 0x1EEE)
		return false;

	if (packet->checksum != checksum(&recvbuf[pos], sizeof(AVRPacket)-1)) // -1 to not include the checksum byte in the checksum
		return false;

	return true;
}

uint8_t XMegaComm::checksum(const uint8_t *data, int length) {
    int sum=0;
    while (length--)
        sum += *data++;
    return (uint8_t)sum;
}

