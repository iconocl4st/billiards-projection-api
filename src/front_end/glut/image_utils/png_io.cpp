/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#define PNG_DEBUG 3
#include <png.h>
#include <GL/gl.h>

#include "front_end/glut/image_utils/png_io.h"

namespace billiards::util{

	void error_(const char * s, ...)
	{
		va_list args;
		va_start(args, s);
		vfprintf(stderr, s, args);
		fprintf(stderr, "\n");
		va_end(args);
// 	abort();
	}

	PngData::~PngData() {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);

		for (int y = 0; y < height; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);
	}


	PngData *read_png_file(const char* file_name)
	{
		char header[8];    // 8 is the maximum size that can be checked

		/* open file and test for it being a png */
		FILE *fp = fopen(file_name, "rb");
		if (!fp) {
			error_("[read_png_file] File %s could not be opened for reading", file_name);
			return nullptr;
		}

		fread(header, 1, 8, fp);
		if (png_sig_cmp((png_const_bytep) header, 0, 8)) {
			fclose(fp);
			error_("[read_png_file] File %s is not recognized as a PNG file", file_name);
			return nullptr;
		}

		auto* img = new PngData{};
		img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (!img->png_ptr) {
			fclose(fp);
			delete img;
			error_("[read_png_file] png_create_read_struct failed");
			return nullptr;
		}

		img->info_ptr = png_create_info_struct(img->png_ptr);
		if (!img->info_ptr) {
			fclose(fp);
			delete img;
			error_("[read_png_file] png_create_info_struct failed");
			return nullptr;
		}

		if (setjmp(png_jmpbuf(img->png_ptr))) {
			fclose(fp);
			delete img;
			error_("[read_png_file] Error during init_io");
			return nullptr;
		}

		png_init_io(img->png_ptr, fp);
		png_set_sig_bytes(img->png_ptr, 8);

		png_read_info(img->png_ptr, img->info_ptr);

		img->width = png_get_image_width(img->png_ptr, img->info_ptr);
		img->height = png_get_image_height(img->png_ptr, img->info_ptr);
		img->color_type = png_get_color_type(img->png_ptr, img->info_ptr);
		img->bit_depth = png_get_bit_depth(img->png_ptr, img->info_ptr);

		img->number_of_passes = png_set_interlace_handling(img->png_ptr);
		png_read_update_info(img->png_ptr, img->info_ptr);

		if (setjmp(png_jmpbuf(img->png_ptr))) {
			fclose(fp);
			delete img;
			error_("[read_png_file] Error during read_image");
			return nullptr;
		}

		img->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * img->height);
		for (int y = 0; y < img->height; y++) {
			img->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(img->png_ptr, img->info_ptr));
		}

		png_read_image(img->png_ptr, img->row_pointers);

		fclose(fp);

		return img;
	}


	void PngData::write_to(const char* file_name) const
	{
		/* create file */
		FILE *fp = fopen(file_name, "wb");
		if (!fp) {
			error_("[write_png_file] File %s could not be opened for writing", file_name);
			return;
		}

		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr) {
			fclose(fp);
			error_("[write_png_file] png_create_write_struct failed");
			return;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			fclose(fp);
			png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
			error_("[write_png_file] png_create_info_struct failed");
			return;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			fclose(fp);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			error_("[write_png_file] Error during init_io");
			return;
		}

		png_init_io(png_ptr, fp);

		/* write header */
		if (setjmp(png_jmpbuf(png_ptr))) {
			fclose(fp);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			error_("[write_png_file] Error during writing header");
			return;
		}

		png_set_IHDR(
			png_ptr,
			info_ptr,
			width, height,
			bit_depth, color_type,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);

		/* write bytes */
		if (setjmp(png_jmpbuf(png_ptr))) {
			fclose(fp);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			error_("[write_png_file] Error during writing bytes");
			return;
		}

		png_write_image(png_ptr, row_pointers);

		/* end write */
		if (setjmp(png_jmpbuf(png_ptr))) {
			fclose(fp);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			error_("[write_png_file] Error during end of write");
			return;
		}

		png_write_end(png_ptr, nullptr);

		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
	}
}