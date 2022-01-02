//
// Created by thallock on 9/20/21.
//

#ifndef IDEA_PATHFILLER_H
#define IDEA_PATHFILLER_H


#include "billiards_common/geometry/triangulation/Map.h"

namespace billiards::qt {

// TODO: Make this depend on the radius....
#define SPHERE_RESOLUTION 20

	class PathFiller {
	public:
		const geometry::TriangulationMap& mapping;
		QPainter& painter;
		QPainterPath path;
		QBrush brush;
		bool atFirst;

		PathFiller(const geometry::TriangulationMap& mapping, QPainter& painter, const graphics::Color& color)
			: mapping{mapping}, painter{painter}, path{}, brush{
			QColor(
				(int) (color.red),
				(int) (color.green),
				(int) (color.blue),
				(int) (color.alpha)),
			Qt::SolidPattern}, atFirst{true} {}

		void raw_vertex(const geometry::Point& center) {
			if (atFirst) {
				path.moveTo(center.x, center.y);
				atFirst = false;
			} else {
				path.lineTo(center.x, center.y);
			}
		}

		void vertex(const geometry::Point& center) {
			raw_vertex(mapping.map(center));
		}

		void setColor(const graphics::Color& color) {
			brush.setColor(QColor(
				(int) (color.red),
				(int) (color.green),
				(int) (color.blue),
				(int) (color.alpha)));
		}

		void fill() {
			painter.fillPath(path, brush);
		}
	};
}
#endif //IDEA_PATHFILLER_H
