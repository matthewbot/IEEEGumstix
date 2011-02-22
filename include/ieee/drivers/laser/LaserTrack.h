#ifndef LASERTRACK_H
#define LASERTRACK_H

#include "ieee/drivers/laser/Image.h"
#include <vector>
#include <stdint.h>

namespace ieee {
	class LaserTrack {
		public:
			struct Config {
				int gmult;
				int brmult;
				int minval;
				int maxpoints;
				int lasersep;
			};

			typedef std::vector<int> LineData;
			typedef std::vector<LineData> LineVec;

			LaserTrack(const Config &config, const Image &frame);

			inline const LineVec &getLineVec() const { return linevec; }
			inline const LineData &getLine(int num) const { return linevec[num]; }

			Image generateGreenChannel() const;

		private:
			const Config &config;
			const Image &frame;
			LineVec linevec;

			void scanCol(int col);

			int pixVal(const uint8_t *pix) const;
	};
}

#endif
