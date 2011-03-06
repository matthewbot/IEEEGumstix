#ifndef WHEELSCONTROL_H
#define WHEELSCONTROL_H

#include "ieee/drivers/avr/SerialPackets.h"
#include <stdint.h>

namespace ieee {
	class WheelsControl {
		public:
			struct WheelConfig {
				int16_t minstop;
				int16_t maxstop;
				int16_t offset;
			};

			struct Config {
				WheelConfig left, right, back;

				int16_t turnhysteresis;
			};

			struct WheelOutput {
				float angle;
				float effort; // 0 to 1
				bool enabled;
			};

			struct Output {
				WheelOutput left, right, back;

				WheelOutput &getWheel(int num); // 0=left, 1=right, 2=back
			};

			WheelsControl(const Config &config);

			void writeOutput(const Output &output, GumstixPacket &gp) const;
			Output readOutput(const AVRPacket &ap) const;

		private:
			const Config &config;

			int16_t toRawAngle(float angle, int16_t curpos, const WheelConfig &wconf, bool &reverse) const;
			static int16_t toRawEffort(float effort, bool reverse);

			static float fromRawAngle(int16_t angle, int16_t effort, const WheelConfig &wconf);
			static float fromRawEffort(int16_t effort);
	};
}

#endif

