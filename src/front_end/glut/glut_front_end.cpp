//
// Created by thallock on 9/15/21.
//


#include "./GlutFrontEnd.h"

namespace locals {
	billiards::gl::GlutFrontEnd *glut_front_end;

	void render() {
		glut_front_end->render();
	}

	void reshape(int w, int h) {
		glut_front_end->reshape(w, h);
	}
}

namespace billiards::project {
	FrontEnd *create_front_end(int& argc, char **argv) {
		locals::glut_front_end = new gl::GlutFrontEnd();
		glutInit(&argc, argv);
		glutInitWindowSize(1920 / 2, 1080 / 2);

		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
		glutCreateWindow("A texture");

		glutDisplayFunc(locals::render);
		glutReshapeFunc(locals::reshape);
		return locals::glut_front_end;
	}
}


