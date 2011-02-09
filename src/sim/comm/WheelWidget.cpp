#include "ieee/sim/comm/WheelWidget.h"
#include <algorithm>

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(WheelWidget, wxPanel)
	EVT_PAINT(WheelWidget::OnPaint)
	EVT_LEFT_DOWN(WheelWidget::OnMouseEvents)
	EVT_MOTION(WheelWidget::OnMouseEvents)
END_EVENT_TABLE()

WheelWidget::WheelWidget(wxWindow *parent)
: wxPanel(parent), dir(M_PI/2+.4), speed(0) { }

void WheelWidget::OnPaint(wxPaintEvent &ev) {
	wxPaintDC dc(this);
	wxSize size = dc.GetSize();

	const int centerx = size.GetWidth()/2;
	const int centery = size.GetHeight()/2;
	const int radius = min(centerx, centery)-2;
	dc.DrawCircle(centerx, centery, radius);

	const float length = radius * 0.65f;
	const float width = max(radius/15.0f, 2.0f);
	const float topx = centerx + length*cos(dir);
	const float topy = centery + length*-sin(dir);
	const float bottomx = centerx - length*cos(dir);
	const float bottomy = centery - length*-sin(dir);

	wxPoint points[4];
	points[0].x = topx + width*cos(dir+M_PI/2);
	points[0].y = topy + width*-sin(dir+M_PI/2);
	points[1].x = topx + width*cos(dir-M_PI/2);
	points[1].y = topy + width*-sin(dir-M_PI/2);
	points[2].x = bottomx + width*cos(dir-M_PI/2);
	points[2].y = bottomy + width*-sin(dir-M_PI/2);
	points[3].x = bottomx + width*cos(dir+M_PI/2);
	points[3].y = bottomy + width*-sin(dir+M_PI/2);

	dc.DrawPolygon(4, points);
}

void WheelWidget::OnMouseEvents(wxMouseEvent &evt) {
	if (!evt.LeftIsDown()) {
		evt.Skip();
		return;
	}

	wxSize size = GetSize();
	const int centerx = size.GetWidth()/2;
	const int centery = size.GetHeight()/2;

	dir = atan2(-(evt.GetY() - centery), evt.GetX() - centerx);
	Refresh();
}

