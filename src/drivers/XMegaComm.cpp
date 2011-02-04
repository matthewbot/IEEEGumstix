#include "ieee/drivers/XMegaComm.h"
#include <cstring>

using namespace ieee;
using namespace boost::posix_time;
using namespace std;

XMegaComm::XMegaComm(const string &device, time_duration timeout, int recvbuflen)
: port(device), timeout(timeout), recvbuf(recvbuflen), recvbufpos(0) {
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
	int got = port.read(&recvbuf[recvbufpos], recvbuf.size() - recvbufpos); // attempt to read more data
	recvbufpos += got;

	int maxpos = recvbufpos - sizeof(AVRPacket); // determine the farthest back a packet could be based on size
	if (maxpos < 0) // if we don't have enough bytes for a new packet
		return false; // then we've already failed

	int pos;
	for (pos=maxpos; pos>=0; pos--) { // start from there and work your way to the front of the buffer looking for a packet
		if (checkRecvbufPacket(pos))
			break;
	}

	int dropamt;
	if (pos >= 0) { // if we found a valid packet
		memcpy(&avrpacket, &recvbuf[0], sizeof(AVRPacket)); // copy it over our previous stored packet
		dropamt = maxpos+sizeof(AVRPacket); // drop all the bytes that made up the packet and all bytes before it

		lastpacket = microsec_clock::local_time();
	} else { // no valid packet
		dropamt = maxpos+1; // drop enough bytes so we're one byte short of a packet again
	}

	memmove(&recvbuf[0], &recvbuf[dropamt], recvbufpos - dropamt); // drop the desired amount of bytes
	recvbufpos -= dropamt;

	return pos >= 0; // return whether we found a valid packet
}

void XMegaComm::syncOut() {
	port.write(reinterpret_cast<uint8_t *>(&gumstixpacket), sizeof(gumstixpacket));
}

bool XMegaComm::checkRecvbufPacket(int pos) {
	AVRPacket *packet = reinterpret_cast<AVRPacket *>(&recvbuf[pos]);
	if (packet->header != 0x1EEE)
		return false;

	if (packet->checksum != checksum(&recvbuf[pos], sizeof(AVRPacket)))
		return false;

	return true;
}

uint8_t XMegaComm::checksum(const uint8_t *data, int length) {
    int sum=0;
    while (length--)
        sum += *data++;
    return (uint8_t)sum;
}

