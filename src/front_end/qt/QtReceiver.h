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


	class Receiver : public graphics::GraphicsReceiver {
	public:
		const project::RenderLocation& location;
		QPainter& painter;

		Receiver(const project::RenderLocation& location, QPainter& painter)
			: location{location}, painter{painter} {}

		~Receiver() = default;

		void fill(const graphics::Circle *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (int i=0; i<SPHERE_RESOLUTION; i++) {
				double a = 2 * M_PI * i / (SPHERE_RESOLUTION - 1);
				p.vertex(graphics->center + geometry::Point{
					graphics->radius * std::cos(a), graphics->radius * std::sin(a)});
			}
			p.fill();
		}
		void draw(const graphics::Circle *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (int i=0; i<SPHERE_RESOLUTION; i++) {
				double a = 2 * M_PI * i / (SPHERE_RESOLUTION - 1);
				p.vertex(graphics->center + geometry::Point{
					graphics->radius * std::cos(a), graphics->radius * std::sin(a)});
			}
			p.fill();
		}
		void fill(const graphics::Polygon *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (auto& vertex : graphics->vertices) {
				p.vertex(vertex);
			}
			p.fill();
		}
		void draw(const graphics::Polygon *graphics) const override {
			PathFiller p{location, painter, graphics->color};
			for (auto& vertex : graphics->vertices) {
				p.vertex(vertex);
			}
			p.fill();
		}
		void fill(const graphics::Lines *graphics) const override {

		}
		void draw(const graphics::Lines *graphics) const override {

		}
		void draw(const graphics::Image *graphics) const override {

		}
		void draw(const graphics::Text *graphics) const override {

		}



		void fill_segment(
			const graphics::Color &color,
			double line_width,
			const geometry::Point& p1, const geometry::Point& p2,
			bool round1, bool round2
		) const {
			geometry::Point d = p2 - p1;
			double dist = d.norm();
			if (dist < TOLERANCE) {
				return;
			}
			geometry::Point nd = d / dist;
			geometry::Point orth{nd.y, -nd.x};
			geometry::Point rorth = orth * line_width;

			PathFiller p{location, painter, color};
			p.vertex(p1 - rorth);
			p.vertex(p2 - rorth);
			if (round2) {
				double a0 = atan2(orth.y, orth.x);
				for (int i = 0; i < SPHERE_RESOLUTION; i++) {
					geometry::Point r{cos(a0 - M_PI * i / SPHERE_RESOLUTION),
									  sin(a0 - M_PI * i / SPHERE_RESOLUTION)};
					p.vertex(p2 - r * line_width);
				}
			}
			p.vertex(p2 + rorth);
			p.vertex(p1 + rorth);
			if (round1) {
				double a0 = atan2(orth.y, orth.x);
				for (int i = 0; i < SPHERE_RESOLUTION; i++) {
					geometry::Point r{cos(M_PI + a0 - M_PI * i / SPHERE_RESOLUTION),
									  sin(M_PI + a0 - M_PI * i / SPHERE_RESOLUTION)};
					p.vertex(p1 - r * line_width);
				}
			}
			p.fill();
		}

		inline
		void draw_oval(
			const graphics::Color &color,
			double line_width,
			const geometry::Point &center,
			double rx, double ry,
			double begin_angle, double end_angle)
		{
			geometry::Point prev_outer;
			geometry::Point prev_inner;
			bool has_prev = false;

			for (int i=0; i<SPHERE_RESOLUTION; i++) {
				double a = begin_angle + i * (end_angle - begin_angle) / (SPHERE_RESOLUTION - 1);
				geometry::Point dir{rx * std::cos(a), ry * std::sin(a)};
				geometry::Point normal{ry * std::cos(a), rx * std::sin(a)};
				double nnorm = normal.norm();
				if (nnorm < TOLERANCE) {
					has_prev = false;
					continue;
				}
				geometry::Point nnormal = normal * (line_width / nnorm);

				geometry::Point cur_inner = location.map(center + dir - nnormal);
				geometry::Point cur_outer = location.map(center + dir + nnormal);

				if (has_prev) {
					PathFiller p{location, painter, color};
					p.vertex(prev_inner);
					p.vertex(prev_outer);
					p.vertex(cur_outer);
					p.vertex(cur_inner);
					p.fill();
				}

				prev_inner = cur_inner;
				prev_outer = cur_outer;
				has_prev = true;
			}
		}
	};
}
#endif //IDEA_QTRECEIVER_H
