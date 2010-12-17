#ifndef OBJECTPANELLAYER_H
#define OBJECTPANELLAYER_H

#include "ieee/sim/World.h"
#include "ieee/sim/WorldPanelLayer.h"

namespace ieee {
	class ObjectPanelLayer : public WorldPanelLayer {
		public:
			struct Callbacks {
				public:
					virtual bool onWorldClicked(const Pos &pos) =0; // return true to begin dragging
					virtual void onWorldDragged(const Pos &pos) =0;

					virtual bool isVictimIdentified(const Pos &pos) =0;
			};

			ObjectPanelLayer(const World &world, const CoordScale &gridscale, Callbacks &callbacks);

			static const int WEIGHT = 8;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

			virtual bool leftDown(const Coord &coord);
			virtual void leftUp();
			virtual void mouseMotion(const Coord &coord);

		private:
			const World &world;
			const CoordScale &gridscale;
			Callbacks &callbacks;

			bool dragging;
			Pos lastdragpos;
	};
}

#endif
