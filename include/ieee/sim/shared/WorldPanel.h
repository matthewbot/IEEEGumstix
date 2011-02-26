#ifndef WORLDPANEL_H
#define WORLDPANEL_H

#include "ieee/sim/shared/WorldPanelLayer.h"
#include <wx/wx.h>

namespace ieee {
	class WorldPanel : public wxPanel {
		public:
			WorldPanel(wxWindow *parent, float roomwidth, float roomheight);

			void addLayer(WorldPanelLayer *layer); // WorldPanel doesn't take ownership
			bool hasLayer(WorldPanelLayer *layer);
			void removeLayer(WorldPanelLayer *layer);

		private:
			CoordScale getDrawScale() const;
			float roomwidth, roomheight;

			typedef std::vector<WorldPanelLayer *> LayerVec;
			LayerVec layers;

			void OnPaint(wxPaintEvent &event);
			void OnLeftDown(wxMouseEvent &event);
			void OnLeftUp(wxMouseEvent &event);
			void OnMotion(wxMouseEvent &event);

			DECLARE_EVENT_TABLE()
	};
}

#endif
