#ifndef IMAGE_H
#define IMAGE_H

// might actually belong in shared

#include <vector>
#include <stdint.h>
#include <cassert>

namespace ieee {
	class Image {
		public:
			enum Format {
				GRAYSCALE=1,
				YUYV=2,
				RGB=3
			};

			// CLEVER/STUPID HACK: the format's enum value is also the number of bytes per pixel
			static int bytesPerPixel(Format f) { return f; }

			Image(Format format=RGB);
			Image(int rows, int cols, Format format=RGB);
			Image(const Image &orig);
			Image(const Image &orig, Format format); // performs conversions

			void resize(int rows, int cols); // destructive resize
			void reformat(int rows, int cols, Format format); // destructive

			inline int getRows() const { return rows; }
			inline int getCols() const { return cols; }
			inline Format getFormat() const { return format; }

			uint8_t *getData() { return &data[0]; }
			const uint8_t *getData() const { return &data[0]; }
			int getDataLength() const { return data.size(); }

			bool inBounds(int row, int col) const;
			inline uint8_t *getPixel(int row, int col) {
				assert(inBounds(row, col));
				return &data[(col+row*cols)*bytesPerPixel(format)];
			}
			inline const uint8_t *getPixel(int row, int col) const {
				assert(inBounds(row, col));
				return &data[(col+row*cols)*bytesPerPixel(format)];
			}

			void clear();

		private:
			int rows, cols;
			Format format;

			typedef std::vector<uint8_t> DataVec;
			DataVec data;

			static void yuyv2rgb(DataVec::const_iterator begin, DataVec::const_iterator end, DataVec::iterator out);
			static void grayscale2rgb(DataVec::const_iterator begin, DataVec::const_iterator end, DataVec::iterator out);
	};
}

#endif

