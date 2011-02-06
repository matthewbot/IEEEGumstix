#include "ieee/drivers/SerialPort.h"
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include <sstream>

using namespace ieee;
using namespace std;

SerialPort::SerialPort(const string &device) {
	if (device.size())
		openDevice(device);
	else
		scanDevices();

	if (tcflush(portfd, TCIFLUSH) == -1)
		throwError("Error while clearing read buffers on serial port");

	termios tio;
	memset(&tio, 0, sizeof(termios));
	tio.c_cflag = B230400 | CS8 | CREAD | CLOCAL; // baud rate, 8 data bits, enable receiver, ignore modem status lines
	tio.c_cc[VTIME] = 0; // no timeout
	tio.c_cc[VMIN] = 0; // don't block
	if (tcsetattr(portfd, TCSANOW, &tio) == -1) // change the port's configuration
		throwError("Error while configuring serial port");
}

SerialPort::~SerialPort() {
	close(portfd);
}

size_t SerialPort::read(uint8_t *buf, size_t size) {
	int amt = ::read(portfd, buf, size);
	if (amt == -1)
		throwError("Failed to read from serial port");
	return (size_t)amt;
}

void SerialPort::write(const uint8_t *buf, int len) {
	while (len > 0) {
		int amt = ::write(portfd, buf, len); // write is not guaranteed to write all of the data in one call
		if (amt == -1)
			throwError("Error while writing to serial port");

		len -= amt;
		buf += amt;
	}
}

void SerialPort::openDevice(const std::string &device) {
	portfd = open(device.c_str(), O_RDWR | O_NOCTTY); // readwrite, don't become controlling TTY, don't wait for DCD line to be asserted
	if (portfd == -1)
		throwError("Failed to open " + device);
}

void SerialPort::scanDevices() {
	stringstream errbuf;

	for (int i=5; i>=0; i--) {
		try {
			stringstream devname;
			devname << "/dev/ttyUSB" << i;
			openDevice(devname.str());
			return;
		} catch (exception &ex) {
			errbuf << ex.what() << endl;
		}
	}

	throw runtime_error("SerialPort failed to scan for device:\n" + errbuf.str());
}

void SerialPort::throwError(const std::string &msg) {
	stringstream buf;
	buf << msg << ": " << strerror(errno);
	throw runtime_error(buf.str());
}

