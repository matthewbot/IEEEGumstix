#include "ieee/drivers/XMegaComm.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace std;

int main(int argc, char **argv) {
	XMegaComm comm;
	cout << "Started XMegaComm" << endl;

	comm.getGumstixPacket().leftwheel_angle = 42;
	while (true) {
		this_thread::sleep(posix_time::milliseconds(100));
		bool good = comm.sync();

		if (good)
			cout << "Got a good packet! Debug output: " << comm.getAVRPacket().debugoutput << endl;
		else
			cout << "Failed to sync" << endl;
	}
}

