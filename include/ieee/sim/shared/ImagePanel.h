#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include "ieee/sim/shared/WorldPanelLayer.h"
#include "ieee/drivers/Image.h"

namespace ieee {
	class ImagePanel : public wxPanel {
		public:
			ImagePanel(wxWindow *parent);

			void update(const Image &frame);

			inline int getFrameWidth() const { return frame.getCols(); }
			inline int getFrameHeight() const { return frame.getRows(); }

		protected:
			void OnPaint(wxPaintEvent &paint);

		private:
			Image frame;
			mutable wxBitmap bitmap;
			mutable bool bitmap_dirty;

			void regenBitmap() const;
			static void frameConvGray(wxImage &out, const Image &frame);
			static void frameConvRGB(wxImage &out, const Image &frame);

			DECLARE_EVENT_TABLE()
	};
}

#endif

