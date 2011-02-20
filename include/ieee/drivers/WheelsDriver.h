#ifndef IEEE_WHEELSDRIVER_H
#define IEEE_WHEELSDRIVER_H

#include "ieee/drivers/SerialPackets.h"
#include <stdint.h>

namespace ieee {
	class WheelsDriver {
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

			WheelsDriver(const Config &config);

			void writeOutput(const Output &output, GumstixPacket &gp);

		private:
			const Config &config;

			static int16_t toRawAngle(float angle);
			static int16_t toRawSpeed(float effort, float angle);
	};
}

#endif

