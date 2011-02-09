#ifndef WHEELWIDGET_H
#define WHEELWIDGET_H

#include <wx/wx.h>

namespace ieee {
	class WheelWidget : public wxPanel {
		public:
			class Callbacks {
				public:
					virtual void onWheelChanged(WheelWidget *widget)=0;
			};

			WheelWidget(wxWindow *parent, Callbacks &callbacks);

			inline float getDirection() const { return dir; } // 0 - 2PI
			void setDirection(float dir);
			inline float getSpeed() const { return speed; } // 0 - 1
			void setSpeed(float speed);

		private:
			Callbacks &callbacks;
			float dir;
			float speed;

			DECLARE_EVENT_TABLE()

			void OnPaint(wxPaintEvent &paint);
			void paintWheel(wxPaintDC &dc);
			void paintSpeedbar(wxPaintDC &dc);

			void OnMouseEvents(wxMouseEvent &evt);
			bool checkWheelClick(wxMouseEvent &evt);
			bool checkSpeedbarClick(wxMouseEvent &evt);

			wxPoint getCircleCenter() const;
			wxRect getSpeedbarRect() const;
			static float doSnap(float val, float tolerance, float stepsize);
	};
}

#endif
