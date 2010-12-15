#ifndef WORLDGRID_H
#define WORLDGRID_H

#include <boost/scoped_array.hpp>
#include "types.h"

namespace ieee {
	class WorldGrid {
		public:
			enum GridSquare {
				EMPTY,
				SMALL_OBSTACLE,
				VICTIM,
				LARGE_OBSTACLE,
				UNKNOWN
			};
		
			WorldGrid(int w, int h, GridSquare clearsquare=EMPTY);
			void clear(GridSquare square=EMPTY);
			
			inline int getWidth() const { return width; }
			inline int getHeight() const { return height; }
			
			inline bool inBounds(const Pos &pos) const {
				if (pos.x < 0 || pos.x >= width)
					return false;
				if (pos.y < 0 || pos.y >= height)
					return false;
				return true;			
			}
			
			inline GridSquare &operator[](const Pos &pos) {
				assert(inBounds(pos));
				return squares[pos.x + pos.y*width];
			}
			inline GridSquare operator[](const Pos &pos) const {
				assert(inBounds(pos));
				return squares[pos.x + pos.y*width];
			}
			
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
