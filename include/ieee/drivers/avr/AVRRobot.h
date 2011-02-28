#ifndef AVRROBOT_H
#define AVRROBOT_H

#include "ieee/drivers/avr/AVRComm.h"
#include "ieee/drivers/avr/CompassSensor.h"
#include "ieee/drivers/avr/SonarSensor.h"
#include "ieee/drivers/avr/WheelsControl.h"

namespace ieee {
	class AVRRobot {
		public:
			typedef WheelsControl::Output WheelsOutput;
			typedef SonarSensor::Reading SonarReading;

			struct Config {
				CompassSensor::Config compass;
				SonarSensor::Config sonar1;
				SonarSensor::Config sonar2;
				WheelsControl::Config wheels;
			};

			AVRRobot(const Config &config);

			bool syncIn();
			void syncOut();

			void disableAll(); // disables servos, steppers, and lasers

			float getCompassAngle() const;
			void calibrateCompassOffset(float desireddir=0);

			SonarReading getSonar1() const;
			SonarReading getSonar2() const;

			void setWheels(const WheelsOutput &wheels);
			WheelsOutput getCurrentWheels() const;

			void setSonarAngle(float angle);
			void setStepperEnabled(bool enabled=true);

			void setUpDown(bool up);

			inline GumstixPacket &getGumstixPacket() { return gp; }
			inline const GumstixPacket &getGumstixPacket() const { return gp; }
			inline const AVRPacket &getAVRPacket() { return ap; }

		private:
			AVRComm comm;
			GumstixPacket gp;
			AVRPacket ap;

			CompassSensor compass;
			SonarSensor sonar1;
			SonarSensor sonar2;
			WheelsControl wheelscontrol;
	};
}

#endif
