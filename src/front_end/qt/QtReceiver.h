//
// Created by thallock on 9/20/21.
//

#ifndef IDEA_QTRECEIVER_H
#define IDEA_QTRECEIVER_H

#include <QPainter>
#include <QPainterPath>

#include "common/graphics/GraphicsReceiver.h"
#include "PathFiller.h"

namespace billiards::qt {


	class Receiver : public gphx::GraphicsReceiver {
	public:
		const project::RenderLocation& location;
		QPainter& painter;

		Receiver(const project::RenderLocation& location, QPainter& painter)
			: location{location}, painter{painter} {}

		~Receiver() = default;

		void receive(const gphx::Circle *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (int i=0; i<SPHERE_RESOLUTION; i++) {
				double a = 2 * M_PI * i / (SPHERE_RESOLUTION - 1);
				p.vertex(graphics->center + geometry::Point{
					graphics->radius * std::cos(a), graphics->radius * std::sin(a)});
			}
			p.fill();
		}

		void receive(const gphx::Polygon *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (auto& vertex : graphics->vertices) {
				p.vertex(vertex);
			}
			p.fill();
		}

		void receive(const gphx::Lines *graphics) const override {

		}
	};
}
#endif //IDEA_QTRECEIVER_H
