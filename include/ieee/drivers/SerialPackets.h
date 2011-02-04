#ifndef SERIALPACKETS_H
#define SERIALPACKETS_H

#include <stdint.h>

namespace ieee {
	struct AVRPacket {
		uint16_t header; // always 0x1EEE
		uint8_t  protoversion; // take version from top of protocol.txt multiply by 10.

		// signed running totals of mouse sensor data. Wrap around is ok.
		int32_t leftmouse_x;
		int32_t leftmouse_y;
		int32_t rightmouse_x;
		int32_t rightmouse_y;

		// current orientation of wheels, following the angles from misc notes in protocol.txt
		int16_t leftwheel_angle;
		int16_t rightwheel_angle;
		int16_t backwheel_angle;

		// current speed of wheels
		int16_t leftwheel_speed;
		int16_t rightwheel_speed;
		int16_t backwheel_speed;

		// sonar reading, as a raw, uncalibrated value
		uint16_t sonar1_reading;
		uint16_t sonar2_reading;
		uint16_t sonar_angle;  // angle of sonar1

		// magnetometer readings
		int16_t mag_x;
		int16_t mag_y;
		int16_t mag_z;

		uint8_t ledsensors[3]; // a raw uncalibrated reading from each of the LED sensors

		char debugoutput[32]; // a null-terminated debugging message that the gumstix will put on the LCD/wireless/mission log

		uint8_t checksum; // 8-bit sum of all other bytes
	} __attribute__((__packed__));

	struct GumstixPacket {
		uint16_t header; // always 0x1EEE
		uint8_t  protoversion; // take version from top of protocol.txt multiply by 10.

		// desired wheel angles. These will be smoothly ramped as needed
		int16_t leftwheel_angle;
		int16_t rightwheel_angle;
		int16_t backwheel_angle;

		// desired wheel velocities. These will be smoothly ramped as needed.
		int16_t leftwheel_speed;
		int16_t rightwheel_speed;
		int16_t backwheel_speed;

		uint16_t sonar_angle; // desired sonar angle

		uint8_t laser_bitmask; // a 1 in the nth bit corresponds to the nth laser from the top being active.

		// true if the retractor is desired to be raised, false if not.
		bool retract;

		uint8_t checksum;
	} __attribute__((__packed__));
}

#endif

