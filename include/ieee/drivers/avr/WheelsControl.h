#ifndef IEEE_WHEELSDRIVER_H
#define IEEE_WHEELSDRIVER_H

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
			};

			struct WheelOutput {
				float angle;
				float effort; // 0 to 1
			};

			struct Output {
				WheelOutput left, right, back;
			};

			WheelsControl(const Config &config);

			void writeOutput(const Output &output, GumstixPacket &gp) const;

		private:
			const Config &config;

			static int16_t toRawAngle(float angle, const WheelConfig &wconf);
			static int16_t toRawEffort(float effort, float angle);
	};
}

#endif

