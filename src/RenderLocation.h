//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_RENDERLOCATION_H
#define GLVIEW_RENDERLOCATION_H

#include "billiards_common/geometry/Point.h"
#include "billiards_common/config/TableGeometry.h"

#include "billiards_common/geometry/triangulation/construct_map.h"

namespace billiards::project {

class RenderLocation : public json::Serializable {
public:
	// Correct map
	geometry::TriangulationMap tri_map;

	// Fall back map
//	config::TableDimensions table_dims;
//	geometry::Point of;
//	geometry::Point up;
//	geometry::Point rt;

    RenderLocation() {

		// TODO: elsewhere
		config::TableDimensions dims{};
		config::TableGeometry geometry{dims};
		bool success = updateMap(dims, geometry);
		if (!success) {
			std::cerr << "Unable to initialize the render location" << std::endl;
		}
	}

	~RenderLocation() override = default;





	void to_json(json::SaxWriter& writer) const override {
		writer.begin_object();
//		writer.key("table-dims");
//		table_dims.to_json(writer);
//		writer.key("offset");
//		of.to_json(writer);
//		writer.key("up");
//		up.to_json(writer);
//		writer.key("right");
//		rt.to_json(writer);
		writer.end_object();
	}

	void parse([[maybe_unused]] const nlohmann::json& value, [[maybe_unused]] json::ParseResult& result) override {
//		if (HAS_OBJECT(value, "table-dims")) {
//			PARSE_CHILD(result, value["table-dims"], table_dims);
//		}
//		if (HAS_OBJECT(value, "offset")) {
//			PARSE_CHILD(result, value["offset"], of);
//		}
//		if (HAS_OBJECT(value, "up")) {
//			PARSE_CHILD(result, value["up"], up);
//		}
//		if (HAS_OBJECT(value, "right")) {
//			PARSE_CHILD(result, value["right"], rt);
//		}
	}
};

}


#endif //GLVIEW_RENDERLOCATION_H
