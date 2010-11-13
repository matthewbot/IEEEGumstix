#include "ieeepath/drivers/MouseSensor.h"
#include <boost/thread.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace std;

int main(int argc, char **argv) {
	string device;
	cout << "Mouse device: ";
	cin >> device;
	
	MouseSensor sensor(device);
	
	int totx=0, toty=0;
	while (true) {
		MouseSensor::Reading reading = sensor.getReading();
		totx += reading.dx;
		toty += reading.dy;
		
		cout << totx << " " << toty << endl;
		
		this_thread::sleep(posix_time::milliseconds(100)); 
	}
}

