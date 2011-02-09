#ifndef WHEELWIDGET_H
#define WHEELWIDGET_H

#include <wx/wx.h>

namespace ieee {
	class WheelWidget : public wxPanel {
		public:
			class Callbacks {
				public:
					virtual void onPositionChanged(WheelWidget *widget)=0;
			};

			WheelWidget(wxWindow *parent, Callbacks &callbacks);

			inline float getDirection() const { return dir; } // 0 - 2PI
			inline float getSpeed() const { return speed; } // 0 - 1

		private:
			Callbacks &callbacks;
			float dir;
			float speed;

			DECLARE_EVENT_TABLE()

			void OnPaint(wxPaintEvent &paint);
			void OnMouseEvents(wxMouseEvent &evt);
	};
}

#endif
