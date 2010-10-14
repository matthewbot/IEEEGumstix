#ifndef WORLDGRID_H
#define WORLDGRID_H

#include <boost/scoped_array.hpp>
#include "types.h"

namespace pathsim {
	class WorldGrid {
		public:
			enum GridSquare {
				EMPTY,
				SMALL_OBSTACLE,
				VICTIM,
				LARGE_OBSTACLE,
				UNKNOWN
			};
			
			inline static bool passable(GridSquare square) { return square < VICTIM; }
			inline static bool known(GridSquare square) { return square != UNKNOWN; }
		
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
			inline GridSquare &operator()(int x, int y) { return (*this)[Pos(x, y)]; }
			inline GridSquare operator()(int x, int y) const { return (*this)[Pos(x, y)]; }
			
			inline GridSquare get(const Pos &pos) const { return (*this)[pos]; }
			inline void set(const Pos &pos, GridSquare square) { (*this)[pos] = square; }
			inline GridSquare get(int x, int y) const { return get(Pos(x, y)); }
			inline void set(int x, int y, GridSquare square) { set(Pos(x, y), square); }
			
			void fillLine(Pos start, Pos end, GridSquare square);

			inline bool getPassable(const Pos &pos) const { return passable((*this)[pos]); }
			int countAdjacent(const Pos &pos, GridSquare square, int range=1) const;
			bool getAdjacent(const Pos &pos, GridSquare square, Pos *out, int range=1) const;
			
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
