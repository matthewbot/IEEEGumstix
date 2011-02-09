#include "ieee/sim/comm/WheelWidget.h"
#include <algorithm>

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(WheelWidget, wxPanel)
	EVT_PAINT(WheelWidget::OnPaint)
	EVT_LEFT_DOWN(WheelWidget::OnMouseEvents)
	EVT_MOTION(WheelWidget::OnMouseEvents)
END_EVENT_TABLE()

WheelWidget::WheelWidget(wxWindow *parent, Callbacks &callbacks)
: wxPanel(parent), callbacks(callbacks), dir(M_PI/2), speed(0) { }

void WheelWidget::OnPaint(wxPaintEvent &ev) {
	wxPaintDC dc(this);
	paintWheel(dc);
	paintSpeedbar(dc);
}

void WheelWidget::paintWheel(wxPaintDC &dc) {
	wxSize size = dc.GetSize();

	wxPoint center = getCircleCenter();
	const int radius = min(center.x, center.y)-2;
	dc.DrawCircle(center.x, center.y, radius);

	const float length = radius * 0.65f;
	const float width = max(radius/15.0f, 2.0f);
	const float topx = center.x + length*cos(dir);
	const float topy = center.y + length*-sin(dir);
	const float bottomx = center.x - length*cos(dir);
	const float bottomy = center.y - length*-sin(dir);

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

void WheelWidget::paintSpeedbar(wxPaintDC &dc) {
	wxRect speedbar = getSpeedbarRect();
	dc.DrawRectangle(speedbar.x, speedbar.y, speedbar.width, speedbar.height);

	dc.SetBrush(*wxBLUE);
	dc.SetPen(*wxBLUE);

	const int sbheight = (int)(speed*speedbar.height);
	dc.DrawRectangle(speedbar.x+1, speedbar.y + speedbar.height-sbheight, speedbar.width-2, sbheight);
}

void WheelWidget::OnMouseEvents(wxMouseEvent &evt) {
	if (!evt.LeftIsDown()) {
		evt.Skip();
		return;
	}

	if (checkWheelClick(evt) || checkSpeedbarClick(evt)) {
		callbacks.onWheelChanged(this);
		Refresh();
	} else {
		evt.Skip();
	}
}

bool WheelWidget::checkWheelClick(wxMouseEvent &evt) {
	wxPoint center = getCircleCenter();

	const int mx = evt.GetX() - center.x;
	const int my = evt.GetY() - center.y;

	const int radius = min(center.x, center.y);

	if (sqrt(mx*mx + my*my) > radius)
		return false;

	float dirval = atan2(-my, mx);
	dir = doSnap(dirval, M_PI/24, M_PI/4);

	return true;
}

bool WheelWidget::checkSpeedbarClick(wxMouseEvent &evt) {
	wxRect rect = getSpeedbarRect();
	const int mx = evt.GetX() - rect.x;
	const int my = evt.GetY() - rect.y;
	if (mx < -2 || my < -10 || mx > rect.width+2 || my > rect.height+10) // give it some tolerance, touch screen can be sucky
		return false;

	float speedval = 1 - (float)my/rect.height;
	if (speedval > 1)
		speedval = 1;
	else if (speedval < 0)
		speedval = 0;

	speed = doSnap(speedval, 0.05, 0.25);
}

wxPoint WheelWidget::getCircleCenter() const {
	wxSize size = GetSize();
	const int centerx = size.GetWidth()/2 - 14;
	const int centery = size.GetHeight()/2;
	return wxPoint(centerx, centery);
}

wxRect WheelWidget::getSpeedbarRect() const {
	wxSize size = GetSize();
	const int width = 14;
	const int height = size.GetHeight() * 3 / 4;
	const int x = size.GetWidth() - width - 2;
	const int y = (size.GetHeight() - height) / 2;
	return wxRect(x, y, width, height);
}

float WheelWidget::doSnap(float val, float tolerance, float stepsize) {
	float snapval = round(val / stepsize)*stepsize;
	if (fabs(snapval - val) < tolerance)
		return snapval;
	else
		return val;
}

