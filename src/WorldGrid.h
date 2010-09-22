#ifndef WORLDGRID_H
#define WORLDGRID_H

#include <boost/scoped_array.hpp>

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
			
			GridSquare &operator()(int x, int y);
			GridSquare operator()(int x, int y) const;
			
			inline GridSquare get(int x, int y) const { return (*this)(x, y); }
			inline void set(int x, int y, GridSquare square) { (*this)(x, y) = square; }
			void fillLine(int startx, int starty, int endx, int endy, GridSquare square);

			inline bool getPassable(int x, int y) const { return passable(get(x, y)); }
			int countAdjacent(int x, int y, GridSquare square) const;
			bool getAdjacent(int x, int y, GridSquare square, int *outx=NULL, int *outy=NULL) const;
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
