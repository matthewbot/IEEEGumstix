#include "ieee/sim/comm/CommApp.h"
#include "ieee/sim/comm/CommFrame.h"
#include <iostream>

using namespace ieee;
using namespace std;

IMPLEMENT_APP(CommApp)

CommApp::CommApp() { }

bool CommApp::OnInit() {
	try {
		CommFrame *frame = new CommFrame();
		frame->Show(true);
		SetTopWindow(frame);
		return true;
	} catch (exception &ex) {
		cout << "exception " << ex.what() << endl;
		return false;
	}
}

