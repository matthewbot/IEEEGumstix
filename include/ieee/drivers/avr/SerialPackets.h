#ifndef SERIALPACKETS_H
#define SERIALPACKETS_H

#define PROTOCOL_VERSION 4
#define AVRPACKET_DEBUGOUTPUT_SIZE 32

#include <stdint.h>

namespace ieee {
	struct AVRPacket {
		uint16_t header; // always 0x1EEE
		uint8_t  protoversion;

		// signed running totals of mouse sensor data. Wrap around is ok.
		int32_t leftmouse_x;
		int32_t leftmouse_y;
		int32_t rightmouse_x;
		int32_t rightmouse_y;

		// surface quality data
		uint8_t leftmouse_squal;
		uint8_t rightmouse_squal;

		// current orientation of wheels, 0-3600
		int16_t leftwheel_angle;
		int16_t rightwheel_angle;
		int16_t backwheel_angle;

		// current wheel efforts
		int16_t leftwheel_effort;
		int16_t rightwheel_effort;
		int16_t backwheel_effort;

		// sonar reading, as a raw, uncalibrated value
		uint16_t sonar1_reading;
		uint16_t sonar2_reading;
		uint16_t sonar_angle;  // angle of sonar1

		// magnetometer readings
		int16_t mag_x;
		int16_t mag_y;
		int16_t mag_z;

		uint16_t batt_volts; // battery voltage in millivolts

		char debugoutput[AVRPACKET_DEBUGOUTPUT_SIZE]; // a null-terminated debugging message that the gumstix will put on the LCD/wireless/mission log

		uint8_t checksum; // 8-bit sum of all other bytes
	} __attribute__((__packed__));

	enum {
		ENABLE_SERVOS = 0x01,
		ENABLE_LASERS = 0x02,
		ENABLE_STEPPER = 0x04
	};

	struct GumstixPacket {
		uint16_t header; // always 0x1EEE
		uint8_t  protoversion; // take version from top of protocol.txt multiply by 10.

		uint8_t enable_bits; // enable bits for servos, lasers, and the stepper

		// desired wheel angles
		int16_t leftwheel_angle;
		int16_t rightwheel_angle;
		int16_t backwheel_angle;

		// desired wheel efforts
		int16_t leftwheel_effort;
		int16_t rightwheel_effort;
		int16_t backwheel_effort;

		uint16_t sonar_angle; // desired sonar angle

		bool updown_lift; // true to lift updown, false to lower

		uint8_t checksum;
	} __attribute__((__packed__));
}

#endif

