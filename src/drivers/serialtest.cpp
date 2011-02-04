#include "ieee/drivers/XMegaComm.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace std;

int main(int argc, char **argv) {
	cout << "Opening serial port" << endl;

	XMegaComm comm("/dev/ttyUSB0");

	while (true) {
		this_thread::sleep(posix_time::milliseconds(500));
		bool good = comm.sync();

		if (good)
			cout << "Got a good packet!" << endl;
	}
}

