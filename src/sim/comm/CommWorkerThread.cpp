#include "ieee/sim/comm/CommWorkerThread.h"
#include <cstring>
#include <stdexcept>

using namespace ieee;
using namespace std;

CommWorkerThread::CommWorkerThread(Callbacks &callbacks)
: wxThread(wxTHREAD_JOINABLE),
  callbacks(callbacks),
  stopflag(false) {
	memset(&avrpacket, 0, sizeof(avrpacket));
	memset(&gumstixpacket, 0, sizeof(gumstixpacket));
	gumstixpacket.header = 0x1EEE;
	gumstixpacket.protoversion = 1;

    try {
  		comm.reset(new AVRComm());
  	} catch (runtime_error &err) {
  		strcpy(avrpacket.debugoutput, "Failed to initialize AVRComm");
  		callbacks.onSync();
  	}

	Create();
}

CommWorkerThread::~CommWorkerThread() {
	stop();
}

void CommWorkerThread::start() {
	if (!comm)
		return;
	Run();
}

void CommWorkerThread::stop() {
	if (!comm)
		return;
	stopflag = true;
	Wait();
}

AVRPacket CommWorkerThread::getAVRPacket() const {
	wxCriticalSectionLocker locker(critsect);
	AVRPacket pack = avrpacket;
	return pack;
}

GumstixPacket CommWorkerThread::getGumstixPacket() const {
	wxCriticalSectionLocker locker(critsect);
	GumstixPacket pack = gumstixpacket;
	return pack;
}

void CommWorkerThread::setGumstixPacket(const GumstixPacket &pack) {
	wxCriticalSectionLocker locker(critsect);
	gumstixpacket = pack;
}

#include <iostream>

wxThread::ExitCode CommWorkerThread::Entry() {
	while (true) {
		if (stopflag)
			return 0;

		wxThread::This()->Sleep(50);

		{
			wxCriticalSectionLocker locker(critsect);

			if (!comm->sync(avrpacket, gumstixpacket))
				continue;
		}

		callbacks.onSync();
	}
}

