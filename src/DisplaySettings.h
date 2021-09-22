//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_DISPLAYSETTINGS_H
#define GLVIEW_DISPLAYSETTINGS_H

#include <list>
#include <memory>
#include <mutex>

#include "RenderLocation.h"

#include "common/graphics/GraphicsPrimitive.h"
#include "common/graphics/parse_graphics.h"

namespace billiards::project {

	class DisplayGraphics : public json::Serializable {
	public:
		std::mutex mutex;
		std::list<std::shared_ptr<graphics::GraphicsPrimitive>> current_graphics;

		DisplayGraphics() = default;
		~DisplayGraphics() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_array();
			for (const auto& graphics : current_graphics) {
				graphics->to_json(writer);
			}
			writer.end_array();
		}

		void parse(const nlohmann::json& value) override {
			if (!value.is_array()) {
				return;
			}
			current_graphics.clear();
			for (const auto& obj : value) {
				current_graphics.emplace_back(graphics::parse_graphics(obj));
			}
		}
	};

	class DisplaySettings : public json::Serializable  {
	public:
		DisplayGraphics graphics;
		RenderLocation location;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("graphics");
			graphics.to_json(writer);
			writer.key("location");
			location.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("graphics") && value["graphics"].is_object()) {
				graphics.parse(value["graphics"]);
			}
			if (value.contains("location") && value["location"].is_object()) {
				location.parse(value["location"]);
			}
		}
	};

}



#endif //GLVIEW_DISPLAYSETTINGS_H
