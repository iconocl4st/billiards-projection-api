//
// Created by thallock on 9/15/21.
//

#ifndef GLVIEW_GLUT_FRONT_END_H
#define GLVIEW_GLUT_FRONT_END_H

#include "../DisplaySettings.h"

namespace billiards::project {

	class FrontEnd {
	public:
		DisplaySettings display;

		FrontEnd() = default;
		virtual ~FrontEnd() = default;

		std::mutex& mutex() { return display.graphics.mutex; }

		virtual int loop() = 0;
		virtual void redraw() = 0;
		virtual void shutdown() = 0;
	};

	FrontEnd *create_front_end(int& argc, char **argv);
}



#endif //GLVIEW_GLUT_FRONT_END_H
