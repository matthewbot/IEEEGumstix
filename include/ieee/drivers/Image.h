#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <stdint.h>

namespace ieee {
	class Image {
		public:
			enum Format {
				RGB,
				GRAYSCALE,
				YUYV
			};

			static int bytesPerPixel(Format f);

			Image(Format format=RGB);
			Image(int rows, int cols, Format format=RGB);
			Image(const Image &orig);
			Image(const Image &orig, Format format); // performs conversions

			void resize(int rows, int cols); // destructive resize
			void reformat(int rows, int cols, Format format); // destructive

			inline int getRows() const { return rows; }
			inline int getCols() const { return cols; }

			uint8_t *getData() { return &data[0]; }
			const uint8_t *getData() const { return &data[0]; }
			int getDataLength() const { return data.size(); }

			uint8_t *getPixel(int row, int col);
			const uint8_t *getPixel(int row, int col) const;

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

