#ifndef ROOMGRID_H
#define ROOMGRID_H

#include <boost/scoped_array.hpp>

namespace roomsim {
	class RoomGrid {
		public:
			enum GridSquare {
				EMPTY,
				SMALL_OBSTACLE,
				VICTIM,
				LARGE_OBSTACLE
			};
			
			static bool passable(GridSquare square);
		
			RoomGrid(int w, int h);
			
			GridSquare &operator()(int x, int y);
			GridSquare operator()(int x, int y) const;

			bool isPassable(int x, int y) const;
		private:
			int width, height;
			boost::scoped_array<GridSquare> squares;
	};
}

#endif
