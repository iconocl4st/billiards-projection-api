//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_PNG_IO_H
#define GLVIEW_PNG_IO_H

#define PNG_DEBUG 3
#include <png.h>

namespace billiards::util {

	class PngData {
		public:
		int width, height;
		png_byte color_type;
		png_byte bit_depth;

		png_structp png_ptr;
		png_infop info_ptr;
		int number_of_passes;
		png_bytep *row_pointers;

		PngData() :
			width{0},
			height{0},
			png_ptr{nullptr},
			info_ptr{nullptr},
			row_pointers{nullptr} {}

		virtual ~PngData();

		void write_to(const char* file_name) const;
	};

	PngData *read_png_file(const char* file_name);
}


#endif //GLVIEW_PNG_IO_H
