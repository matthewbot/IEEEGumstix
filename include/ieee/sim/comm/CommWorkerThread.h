#ifndef COMMWORKERTHREAD_H
#define COMMWORKERTHREAD_H

#include "ieee/drivers/XMegaComm.h"
#include <wx/wx.h>
#include <memory>

namespace ieee {
	class CommWorkerThread : wxThread {
		public:
			class Callbacks {
				public:
					virtual void onSync() = 0;
			};

			CommWorkerThread(Callbacks &callbacks);
			~CommWorkerThread();

			void start();
			void stop();

			inline const AVRPacket &getAVRPacket() const { return comm.getAVRPacket(); }
			inline const GumstixPacket &getGumstixPacket() const { return comm.getGumstixPacket(); }
			inline GumstixPacket &getGumstixPacket() { return comm.getGumstixPacket(); }

		private:
			virtual ExitCode Entry();
			Callbacks &callbacks;

			XMegaComm comm;

			volatile bool stopflag;
	};
}

#endif

