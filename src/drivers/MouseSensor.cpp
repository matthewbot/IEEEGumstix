#include "ieee/drivers/MouseSensor.h"
#include <sstream>
#include <linux/input.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <iostream>

using namespace ieee;
using namespace std;

MouseSensor::MouseSensor(const string &path) {
	fd = open(path.c_str(), O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		stringstream buf;
		buf << "Failed to open " << path << ": " << strerror(errno) << endl;
		throw Error(buf.str());
	}
	
	int evtype;
	memset(&evtype, 0, sizeof(evtype));
	if (ioctl(fd, EVIOCGBIT(0, sizeof(evtype)), &evtype) < 0) {
		close(fd);
		stringstream buf;
		buf << "Failed to determine event interface capabilities of " << path << ": " << strerror(errno) << endl;
		throw Error(buf.str());
	}
	
	if (!(evtype & (1 << EV_REL))) {
		close(fd);
		stringstream buf;
		buf << "Device file " << path << " is not a mouse" << endl;
		throw Error(buf.str());
	}
}

MouseSensor::Reading MouseSensor::getReading() {
	Reading reading = { 0, 0 };

	input_event evbuf[32];
	int bytes;
	while ((bytes = read(fd, &evbuf, sizeof(evbuf))) > 0) { // read a batch of events
		if (bytes % sizeof(input_event) != 0)
			throw Error("Got a partial read (HID driver bug?)");

		int count = bytes / sizeof(input_event);
		for (int i=0; i<count; i++) { // loop over each event individually
			const input_event &ev = evbuf[i];
			
			if (ev.type != EV_REL) // if its not a motion event we don't care about it
				continue;
		
			if (ev.code == REL_X)
				reading.dx += ev.value;
			else if (ev.code == REL_Y)
				reading.dy += ev.value;
		}
		
		if (bytes < sizeof(evbuf)) // if read didn't fill our buffer
			break; // don't bother calling read again, it will just tell us there is none left
	}
	
	if (bytes == -1 && errno != EAGAIN) { // EAGAIN is given when no data is available because we're in non-blocking mode, so we don't really care about it
		stringstream buf;
		buf << "Read error: " << strerror(errno);
		throw Error(buf.str());
	}
	
	return reading;
}

