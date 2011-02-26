#include "ieee/sim/control/ControlTestApp.h"
#include "ieee/sim/control/ControlTestFrame.h"
#include <iostream>

using namespace ieee;
using namespace std;

IMPLEMENT_APP(ControlTestApp)

ControlTestApp::ControlTestApp() { }

bool ControlTestApp::OnInit() {
	try {
		ControlTestFrame *frame = new ControlTestFrame();
		frame->Show(true);
		SetTopWindow(frame);
		return true;
	} catch (exception &ex) {
		cout << "exception " << ex.what() << endl;
		return false;
	}
}

