//
// Created by thallock on 9/16/21.
//

#include "./QtFrontEnd.h"

namespace billiards::project {
	FrontEnd *create_front_end(int& argc, char **argv) {
		return new qt::QtFrontEnd(argc, argv);
	}
}


