#include "ieee/drivers/avr/AVRRobot.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace ieee;
using namespace std;

struct AVRRobotConfig : AVRRobot::Config {
	AVRRobotConfig();
};

static void stepWheel(int num, AVRRobot &robot, const std::string &filename);
static void spinInPlace(AVRRobot &robot);
static void prompt();

int main(int argc, char **argv) {
	AVRRobotConfig robotconfig;
	AVRRobot robot(robotconfig);

	robot.setSonarAngle(0);

	stepWheel(0, robot, "mag0.txt");
	stepWheel(1, robot, "mag1.txt");
	stepWheel(2, robot, "mag2.txt");
	spinInPlace(robot);
}

static void stepWheel(int num, AVRRobot &robot, const std::string &filename) {
	ofstream dataout(filename.c_str());
	cout << "Begin wheel test " << num << endl;
	AVRRobot::WheelsOutput output;
	WheelsControl::WheelOutput &wheelout = output.getWheel(num);

	output.left.effort = output.right.effort = output.back.effort = 0;
	output.left.angle = output.right.angle = output.back.angle = M_PI/2;
	wheelout.angle = 0;
	robot.setWheels(output);
	robot.syncIOWait(30);

	const AVRPacket &ap = robot.getAVRPacket();
	int basemagx = ap.mag_x, basemagy = ap.mag_y, basemagz = ap.mag_z;

	while (wheelout.angle < M_PI) {
		robot.syncIOWait(5);

		wheelout.angle += M_PI / 128;
		robot.setWheels(output);

		dataout << wheelout.angle << " " << ap.mag_x - basemagx << " " << ap.mag_y - basemagy << " " << ap.mag_z - basemagz << endl;
	}
	cout << "Wheel test " << num << " done" << endl;
}

static void spinInPlace(AVRRobot &robot) {
	ofstream dataout("spin.txt");

	AVRRobot::WheelsOutput output;
	output.left.effort = output.right.effort = output.back.effort = 0;
	output.left.angle = output.right.angle = output.back.angle = M_PI/2;

	robot.setWheels(output);
	robot.syncIOWait(5);

	cout << "Ready for spin in place test"; prompt();

	const AVRPacket &ap = robot.getAVRPacket();
	for (int i=0; i<100; i++) {
		robot.syncIOWait(5);
		dataout << ap.mag_x << " " << ap.mag_y << " " << ap.mag_z << endl;

		cout << i << endl;
	}
}

static void prompt() {
	string dummy;
	getline(cin, dummy);
}

AVRRobotConfig::AVRRobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = 40;
	wheels.right.offset = 0;
	wheels.back.offset = 70;

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = 57.500;
	compass.centery = 281.5;
	compass.yscale = 1.0644;
}

