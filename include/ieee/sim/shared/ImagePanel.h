#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include "ieee/sim/shared/WorldPanelLayer.h"
#include <opencv/cv.h>

namespace ieee {
	class ImagePanel : public wxPanel {
		public:
			ImagePanel(wxWindow *parent);

			void update(const cv::Mat &frame);

		private:
			cv::Mat frame;
			mutable wxBitmap bitmap;
			mutable bool bitmap_dirty;

			void regenBitmap() const;
			void OnPaint(wxPaintEvent &paint);

			DECLARE_EVENT_TABLE()
	};
}

#endif

