#ifndef AVRROBOT_H
#define AVRROBOT_H

#include "ieee/drivers/avr/AVRComm.h"
#include "ieee/drivers/avr/CompassSensor.h"
#include "ieee/drivers/avr/SonarSensor.h"
#include "ieee/drivers/avr/WheelsControl.h"
#include "ieee/drivers/avr/StepperControl.h"
#include <boost/property_tree/ptree.hpp>

namespace ieee {
	class AVRRobot {
		public:
			typedef WheelsControl::Output WheelsOutput;

			struct Config {
				CompassSensor::Config compass;
				SonarSensor::Config sonar1;
				SonarSensor::Config sonar2;
				WheelsControl::Config wheels;
				StepperControl::Config stepper;

				void readTree(const boost::property_tree::ptree &pt);
			};

			AVRRobot(const Config &config);

			bool syncIn();
			void syncIOWait(int count=1); // throws exception if no sync in 25 ms, repeats full sync count times
			void syncOut();

			void disableAll(); // disables servos, steppers, and lasers

			float getCompassAngle() const;
			void calibrateCompassOffset(float desireddir=0);

			float getSonar1() const;
			float getSonar2() const;

			void setWheels(const WheelsOutput &wheels);
			WheelsOutput getCurrentWheels() const;
			void setWheelAngles(float left, float right, float back);
			void offWheels();

			void setSonarAngle(float angle);
			void setStepperEnabled(bool enabled=true);
			float getCurSonarAngle() const;

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
			StepperControl steppercontrol;
	};
}

#endif
