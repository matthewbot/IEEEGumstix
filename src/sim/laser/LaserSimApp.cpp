#include "ieee/sim/laser/LaserSimApp.h"
#include "ieee/sim/laser/LaserSimFrame.h"
#include <iostream>

using namespace ieee;
using namespace std;

IMPLEMENT_APP(LaserSimApp)

LaserSimApp::LaserSimApp() { }

bool LaserSimApp::OnInit() {
	try {
		LaserSimFrame *frame = new LaserSimFrame();
		frame->Show(true);
		SetTopWindow(frame);
		return true;
	} catch (exception &ex) {
		cout << "exception " << ex.what() << endl;
		return false;
	}
}

