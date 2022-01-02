//
// Created by thallock on 12/22/21.
//

#ifndef IDEA_CONSTRUCT_MAP_H
#define IDEA_CONSTRUCT_MAP_H

#include "billiards_common/geometry/triangulation/Map.h"
#include "billiards_common/config/MappingInfo.h"

#include <Eigen/Dense>

namespace billiards::project {

	[[nodiscard]] inline
	bool construct_map(
		const config::TableGeometry& inputs,
		const config::TableGeometry& outputs,
		const int idx1, const int idx2, const int idx3,
		geometry::TriangularMap& map
	) {
		map.vertices = std::array<geometry::Point, 3>{
			geometry::Point{inputs.points[idx1][0], inputs.points[idx1][1]},
			geometry::Point{inputs.points[idx2][0], inputs.points[idx2][1]},
			geometry::Point{inputs.points[idx3][0], inputs.points[idx3][1]},
		};

		Eigen::Matrix<double, 6, 6> A;
		A <<	map.vertices[0].x, map.vertices[0].y, 1, 0, 0, 0,
				0, 0, 0, map.vertices[0].x, map.vertices[0].y, 1,
				map.vertices[1].x, map.vertices[1].y, 1, 0, 0, 0,
				0, 0, 0, map.vertices[1].x, map.vertices[1].y, 1,
				map.vertices[2].x, map.vertices[2].y, 1, 0, 0, 0,
				0, 0, 0, map.vertices[2].x, map.vertices[2].y, 1;

//		Eigen::Vector<double, 6> b;
		Eigen::Matrix<double, 6, 1> b;
		b <<	outputs.points[idx1][0], outputs.points[idx1][1],
				outputs.points[idx2][0], outputs.points[idx2][1],
				outputs.points[idx3][0], outputs.points[idx3][1];

		/*Eigen::Vector<double, 6>*/ auto a = A.colPivHouseholderQr().solve(b);
		map.a[0][0] = a(0);
		map.a[0][1] = a(1);
		map.a[0][2] = a(2);
		map.a[1][0] = a(3);
		map.a[1][1] = a(4);
		map.a[1][2] = a(5);

		const bool b1 = map.domain[0].orient(
			map.vertices[0].x, map.vertices[0].y,
			map.vertices[1].x, map.vertices[1].y,
			map.vertices[2].x, map.vertices[2].y
		);
		const bool b2 = map.domain[1].orient(
			map.vertices[1].x, map.vertices[1].y,
			map.vertices[2].x, map.vertices[2].y,
			map.vertices[0].x, map.vertices[0].y
		);
		const bool b3 = map.domain[2].orient(
			map.vertices[2].x, map.vertices[2].y,
			map.vertices[0].x, map.vertices[0].y,
			map.vertices[1].x, map.vertices[1].y
		);

		return b1 && b2 && b3;
	}


	[[nodiscard]] inline
	bool construct_map(
		const project::MappingInformation& map_info,
		geometry::TriangulationMap& map
	) {
		int indices[10][3] = {
			{1, 0, 4},
			{1, 4, 6},
			{1, 6, 8},
			{1, 8, 9},
			{1, 9, 2},
			{2, 9, 10},
			{2, 10, 11},
			{2, 11, 7},
			{2, 7, 5},
			{2, 5, 3},
		};

		bool ret = true;
		for (auto i = 0; i < (int) map.maps.size(); i++) {
			ret &= construct_map(
				map_info.physical, map_info.screen,
				indices[i][0], indices[i][1], indices[i][2],
				map.maps[i]);
		}
		return ret;
	}
}

#endif //IDEA_CONSTRUCT_MAP_H
