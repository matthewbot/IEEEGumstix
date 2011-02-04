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

	while (true) {
		this_thread::sleep(posix_time::milliseconds(500));
		bool good = comm.sync();

		if (good)
			cout << "Got a good packet! Debug output: " << comm.getAVRPacket().debugoutput << endl;
		else
			cout << "sync() failed" << endl;
	}
}

