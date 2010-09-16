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
			
			inline int getWidth() const { return width; }
			inline int getHeight() const { return height; }
			
			GridSquare &operator()(int x, int y);
			GridSquare operator()(int x, int y) const;

			bool isPassable(int x, int y) const;
			
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
