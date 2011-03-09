#ifndef LASERTRACK_H
#define LASERTRACK_H

#include "ieee/drivers/laser/Image.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <stdint.h>

namespace ieee {
	class LaserTrack {
		public:
			struct Config {
				int minval;
				int maxpoints;
				int lasersep;

				void readTree(const boost::property_tree::ptree &pt);
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
