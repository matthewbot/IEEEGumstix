#include "ieee/sim/comm/CommWorkerThread.h"

using namespace ieee;

CommWorkerThread::CommWorkerThread(Callbacks &callbacks)
: wxThread(wxTHREAD_JOINABLE),
  callbacks(callbacks),
  comm("/dev/null"),
  stopflag(false) {
	Create();
}

CommWorkerThread::~CommWorkerThread() {
	stop();
}

void CommWorkerThread::start() {
	Run();
}

void CommWorkerThread::stop() {
	stopflag = true;
	Wait();
}

wxThread::ExitCode CommWorkerThread::Entry() {
	while (true) {
		if (stopflag)
			return 0;

		wxThread::This()->Sleep(50);
		if (comm.sync())
			callbacks.onSync();
	}
}

