//
// Created by thallock on 9/15/21.
//

#ifndef GLVIEW_GLUTFRONTEND_H
#define GLVIEW_GLUTFRONTEND_H

#include <GL/glut.h>

#include "../FrontEnd.h"
#include "./gl_render.h"

#include "common/graphics/Circle.h"

namespace billiards::gl {

	class GlutFrontEnd : public billiards::project::FrontEnd {
	public:
		GlutFrontEnd() : FrontEnd{} {}

		void render() {
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (const auto& ptr: display.graphics.current_graphics) {
				std::string type = ptr->get_type();
				if (type == "circle") {
					auto *circle = dynamic_cast<graphics::Circle *>(ptr.get());
					fill_circle(
						display.location,
						circle->color,
						circle->center,
						circle->radius);
				}
			}

			{
				graphics::Color color{0, 10, 255, 127};
				std::array<geometry::Point, 5> points = {
					geometry::Point{0, 0},
					geometry::Point{display.location.table_width, 0},
					geometry::Point{display.location.table_width, display.location.table_height},
					geometry::Point{0, display.location.table_height},
					geometry::Point{0, 0}
				};
				lines(
					display.location,
					color,
					std::begin(points), std::end(points)
				);
			}

			glutSwapBuffers();
		}

		void reshape(int w, int h) {
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, w, 0, h, -1, 1);
			glScalef(1, 1, 1);
		}

		int loop() override {
			glutMainLoop();
			return 0;
		}

		void redraw() override {
			glutPostRedisplay();
		}

		void shutdown() override {

		}
	};

}

#endif //GLVIEW_GLUTFRONTEND_H
