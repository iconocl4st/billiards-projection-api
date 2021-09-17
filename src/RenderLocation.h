//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_RENDERLOCATION_H
#define GLVIEW_RENDERLOCATION_H

#include "common/geometry/Point.h"


namespace billiards::project {

class RenderLocation : public json::Serializable {
public:
	double table_width, table_height;

	geometry::Point of; // offset of table
	geometry::Point up; // straight up on table
	geometry::Point rt; // right on table

	RenderLocation() :
		table_width{92},
		table_height{46},
		of{15, 20},
		up{20, 560},
		rt{1000, -5} {}

	[[nodiscard]] inline
	geometry::Point map(double x, double y) const {
		double xn = x / table_width;
		double yn = y / table_height;
		return geometry::Point{
			of.x + rt.x * xn + up.x * yn,
			of.y + rt.y * xn + up.y * yn
		};
	}

	[[nodiscard]] inline
	geometry::Point map(const geometry::Point& in) const {
		return map(in.x, in.y);
	}

	void to_json(json::SaxWriter& writer) const override {
		writer.begin_object();
		writer.key("table");
		writer.begin_object();
		writer.field("width", table_width);
		writer.field("height", table_height);
		writer.end_object();
		writer.key("offset");
		of.to_json(writer);
		writer.key("up");
		up.to_json(writer);
		writer.key("right");
		rt.to_json(writer);
		writer.end_object();
	}

	void parse(const nlohmann::json& value) override {
		std::cout << "parsing " << value.dump() << std::endl;

		if (value.contains("table")) {
			auto& table_dims = value["table"];
			if (table_dims.contains("width") && table_dims["width"].is_number()) {
				table_width = table_dims["width"].get<double>();
			}
			if (table_dims.contains("height") && table_dims["height"].is_number()) {
				table_height = table_dims["height"].get<double>();
			}
		}
		if (value.contains("offset")) {
			of.parse(value["offset"]);
		}
		if (value.contains("up")) {
			up.parse(value["up"]);
		}
		if (value.contains("right")) {
			rt.parse(value["right"]);
		}
	}
};

}


#endif //GLVIEW_RENDERLOCATION_H
