#ifndef WORLDGRID_H
#define WORLDGRID_H

#include <boost/scoped_array.hpp>

namespace roomsim {
	class WorldGrid {
		public:
			enum GridSquare {
				EMPTY,
				SMALL_OBSTACLE,
				VICTIM,
				LARGE_OBSTACLE
			};
			
			static bool passable(GridSquare square);
		
			WorldGrid(int w, int h);
			void clear();
			
			inline int getWidth() const { return width; }
			inline int getHeight() const { return height; }
			
			GridSquare &operator()(int x, int y);
			GridSquare operator()(int x, int y) const;
			
			inline GridSquare get(int x, int y) const { return (*this)(x, y); }
			inline void set(int x, int y, GridSquare square) { (*this)(x, y) = square; }
			void fillLine(int startx, int starty, int endx, int endy, GridSquare square);

			inline bool getPassable(int x, int y) const { return passable(get(x, y)); }
			
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
