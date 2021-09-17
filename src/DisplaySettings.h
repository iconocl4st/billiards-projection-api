//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_DISPLAYSETTINGS_H
#define GLVIEW_DISPLAYSETTINGS_H

#include <list>
#include <memory>

#include "RenderLocation.h"
#include "common/graphics/GraphicsPrimitive.h"

namespace billiards::project {

	class DisplaySettings {
	public:
		std::list<std::shared_ptr<gphx::GraphicsPrimitive>> current_graphics;
		RenderLocation location;
	};

}



#endif //GLVIEW_DISPLAYSETTINGS_H
