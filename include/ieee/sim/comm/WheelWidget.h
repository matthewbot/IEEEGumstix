#ifndef WHEELWIDGET_H
#define WHEELWIDGET_H

#include <wx/wx.h>

namespace ieee {
	class WheelWidget : public wxPanel {
		public:
			WheelWidget(wxWindow *parent);

			inline float getMotorDirection() const { return dir; } // 0 - 2PI
			inline float getMotorSpeed() const { return speed; } // 0 - 1

		private:
			float dir;
			float speed;

			DECLARE_EVENT_TABLE()

			void OnPaint(wxPaintEvent &paint);
			void OnMouseEvents(wxMouseEvent &evt);
	};
}

#endif
