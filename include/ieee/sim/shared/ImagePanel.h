#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include "ieee/sim/shared/WorldPanelLayer.h"
#include <opencv/cv.h>

namespace ieee {
	class ImagePanel : public wxPanel {
		public:
			ImagePanel(wxWindow *parent);

			void update(const cv::Mat &frame);

			inline int getFrameWidth() const { return frame.cols; }
			inline int getFrameHeight() const { return frame.rows; }

		protected:
			void OnPaint(wxPaintEvent &paint);

		private:
			cv::Mat frame;
			mutable wxBitmap bitmap;
			mutable bool bitmap_dirty;

			void regenBitmap() const;
			static void frameConv1C(wxImage &out, const cv::Mat &frame);
			static void frameConv3C(wxImage &out, const cv::Mat &frame);

			DECLARE_EVENT_TABLE()
	};
}

#endif

