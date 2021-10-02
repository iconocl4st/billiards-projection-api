//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_GLIMAGE_H
#define GLVIEW_GLIMAGE_H

#include <GL/gl.h>

#include "front_end/glut/image_utils/png_io.h"

class GlImage {
public:
	GLsizei width;
	GLsizei height;
	GLubyte *bytes;

	GlImage(GLsizei width, GLsizei height);
	virtual ~GlImage();
};

GlImage *create_gl_image(billiards::util::PngData *img);
GlImage *read_png(const char *filename);

#endif //GLVIEW_GLIMAGE_H
