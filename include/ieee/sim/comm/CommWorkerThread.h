#ifndef COMMWORKERTHREAD_H
#define COMMWORKERTHREAD_H

#include "ieee/drivers/avr/AVRComm.h"
#include <boost/scoped_ptr.hpp>
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

			AVRPacket getAVRPacket() const;
			GumstixPacket getGumstixPacket() const;
			void setGumstixPacket(const GumstixPacket &gp);

		private:
			virtual ExitCode Entry();
			Callbacks &callbacks;

			boost::scoped_ptr<AVRComm> comm;

			mutable wxCriticalSection critsect;
			AVRPacket avrpacket;
			GumstixPacket gumstixpacket;
			volatile bool stopflag;
	};
}

#endif

