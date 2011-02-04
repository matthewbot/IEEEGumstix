#include "ieee/drivers/SerialPortBuf.h"
#include <cstring>
#include <cassert>

using namespace ieee;
using namespace std;

SerialPortBuf::SerialPortBuf(int len) : data(len), count(0) { }

void SerialPortBuf::fill(SerialPort &port) {
	count += port.read(&data[count], data.size() - count);
}

void SerialPortBuf::drop(int amt) {
	assert(count >= amt);

	if (count > amt)
		memmove(&data[0], &data[amt], count - amt); // drop the desired amount of bytes
	count -= amt;
}

