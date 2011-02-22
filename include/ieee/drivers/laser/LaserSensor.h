#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include "ieee/drivers/laser/V4LCapture.h"
#include "ieee/drivers/laser/Image.h"
#include "ieee/drivers/laser/LaserTrack.h"
#include "ieee/shared/types.h"
#include <vector>
#include <memory>

namespace ieee {
	class LaserSensor {
		public:
			typedef LaserTrack::LineVec RawReadings;

			struct DistAngle {
				float dist; // in cm
				float angle;

				inline DistAngle() { }
				inline DistAngle(float dist, float angle) : dist(dist), angle(angle) { }

				Coord toCoord(float rotate=0) const;
			};

			typedef std::vector<DistAngle> DistAngleVec;
			typedef std::vector<DistAngleVec> Readings;

			struct Debug {
				Image rawframe;
				Image greenframe;
				RawReadings rawreadings;
			};

			struct Calibration {
				float alpha;
				float beta;
				float offset;
			};

			struct Config : LaserTrack::Config {
				inline Config() { }

				const Calibration *calibrations;
				int exposure;
				float viewangle; // horizontal, in radians
			};

			LaserSensor(const Config &config, const std::string &devname="");

			RawReadings captureRawReadings(Debug *debug=NULL);
			Readings captureReadings(Debug *debug=NULL);

			inline void setExposure(int exposure) { cap.setExposure(exposure); }

			static std::auto_ptr<LaserSensor> createAndHandleExposureFailure(Config &config, const std::string &devname="");

		private:
			const Config &config;
			V4LCapture cap;

			DistAngle toDistAngle(int row, int col, int maxcol, int laser) const;
	};
}

#endif
