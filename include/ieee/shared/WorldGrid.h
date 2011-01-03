#ifndef WORLDGRID_H
#define WORLDGRID_H

#include <boost/multi_array.hpp>
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

			inline int getWidth() const { return squares.shape()[0]; }
			inline int getHeight() const { return squares.shape()[1]; }

			void resizeClear(int w, int h, GridSquare square=EMPTY);

			inline bool inBounds(const Pos &pos) const {
				if (pos.x < 0 || pos.x >= getWidth())
					return false;
				if (pos.y < 0 || pos.y >= getHeight())
					return false;
				return true;
			}

			inline GridSquare &operator[](const Pos &pos) {
				assert(inBounds(pos));
				return squares[pos.x][pos.y];
			}
			inline GridSquare operator[](const Pos &pos) const {
				assert(inBounds(pos));
				return squares[pos.x][pos.y];
			}

		private:
			boost::multi_array<GridSquare, 2> squares;
	};
}

#endif
