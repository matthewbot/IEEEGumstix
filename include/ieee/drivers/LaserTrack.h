#ifndef LASERTRACK_H
#define LASERTRACK_H

#include <vector>

namespace cv {
	class Mat;
}

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

			LaserTrack(const Config &config, const cv::Mat &frame);

			inline const LineVec &getLineVec() const { return linevec; }
			inline const LineData &getLine(int num) const { return linevec[num]; }

			cv::Mat generateGreenChannel() const;

		private:
			const Config &config;
			const cv::Mat &frame;
			LineVec linevec;

			void scanCol(int col);

			int pixVal(const unsigned char *pix) const;
	};
}

#endif
