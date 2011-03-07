#ifndef IEEE_COMPASSSENSOR_H
#define IEEE_COMAPSSSENSOR_H

#include "ieee/drivers/avr/SerialPackets.h"
#include "ieee/drivers/avr/WheelsControl.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>

namespace ieee {
	class CompassSensor {
		public:
			struct WheelOffsetPoly {
				std::vector<float> magx;
				std::vector<float> magy;

				void readTree(const boost::property_tree::ptree &pt);
			};

			struct Config {
				WheelOffsetPoly leftwheel_offset;
				WheelOffsetPoly rightwheel_offset;
				WheelOffsetPoly backwheel_offset;

				int16_t centerx;
				int16_t centery;
				float yscale; // scale of y axis with respect to x axis

				void readTree(const boost::property_tree::ptree &pt);
			};

			CompassSensor(const Config &config);

			float getAngle(const AVRPacket &ap, const WheelsControl &wheels) const;

			void calibrateOffset(const AVRPacket &ap, const WheelsControl &wheels, float desiredangle);

		private:
			const Config &config;
			float offset;

			static float polyval(const std::vector<float> &coefs, float val);
			static float angleWrap(float angle);
	};
}

#endif
