#ifndef OBJECTPANELLAYER_H
#define OBJECTPANELLAYER_H

#include "ieee/sim/shared/World.h"
#include "ieee/sim/shared/WorldPanelLayer.h"

namespace ieee {
	class ObjectPanelLayer : public WorldPanelLayer {
		public:
			struct Callbacks {
				public:
					virtual bool onWorldClicked(const Coord &coord) =0; // return true to begin dragging
					virtual void onWorldDragged(const Coord &coord) =0;
			};

			ObjectPanelLayer(const World &world, const CoordScale &gridscale, Callbacks &callbacks);

			static const int WEIGHT = 8;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

			virtual bool leftDown(const Coord &coord);
			virtual bool leftUp(const Coord &coord);
			virtual bool mouseMotion(const Coord &coord);

		private:
			const World &world;
			const CoordScale &gridscale;
			Callbacks &callbacks;

			bool dragging;
	};
}

#endif
