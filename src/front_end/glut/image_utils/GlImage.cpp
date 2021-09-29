//
// Created by thallock on 9/5/21.
//

#include "GlImage.h"

#include <cstdlib>
#include <iostream>

GlImage::GlImage(GLsizei width, GLsizei height) :
	width{width},
	height{height},
	bytes{(GLubyte *) malloc(4 * width * height * sizeof(*bytes))} {}

GlImage::~GlImage() {
	free(bytes);
}

GlImage *create_gl_image(billiards::util::PngData *img)
{
	if (png_get_color_type(img->png_ptr, img->info_ptr) == PNG_COLOR_TYPE_RGB) {
		std::cerr << "[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA ";
		std::cerr << "(lacks the alpha channel)\n";
		return nullptr;
	}

	if (png_get_color_type(img->png_ptr, img->info_ptr) != PNG_COLOR_TYPE_RGBA) {
		std::cerr << "[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (";
		std::cerr << PNG_COLOR_TYPE_RGBA << ") (is " << png_get_color_type(img->png_ptr, img->info_ptr) << "\n";
		return nullptr;
	}

	auto *ret = new GlImage((GLsizei) img->width, (GLsizei) img->height);
	GLubyte *offset = ret->bytes;
	for (int y = img->height-1; y >= 0; y--)
	{
		png_byte* row = img->row_pointers[y];
		for (int x=0; x<img->width; x++)
		{
			png_byte* ptr = &(row[x*4]);

			*offset++ = (GLubyte) ptr[0];
			*offset++ = (GLubyte) ptr[1];
			*offset++ = (GLubyte) ptr[2];
			*offset++ = (GLubyte) ptr[3];
		}
	}

	return ret;
}

GlImage *read_png(const char *filename) {
	billiards::util::PngData *png = billiards::util::read_png_file(filename);
	GlImage *img = create_gl_image(png);
	delete png;
	return img;
}