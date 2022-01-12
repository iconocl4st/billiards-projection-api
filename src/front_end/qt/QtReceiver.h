//
// Created by thallock on 9/20/21.
//

#ifndef IDEA_QTRECEIVER_H
#define IDEA_QTRECEIVER_H

#include <QPainter>
#include <QPainterPath>

#include "billiards_common/graphics/GraphicsReceiver.h"
#include "front_end/qt/PathFiller.h"

namespace billiards::qt {

	class Receiver : public graphics::GraphicsReceiver {
	public:
		const geometry::TriangulationMap& mapping;
		QPainter& painter;

		Receiver(const geometry::TriangulationMap& mapping, QPainter& painter)
			: mapping{mapping}, painter{painter} {}

		~Receiver() = default;

		void fill(const graphics::Circle *graphics) const override {
			PathFiller p{mapping, painter, graphics->color};
			for (int i=0; i<SPHERE_RESOLUTION; i++) {
				double a = 2 * M_PI * i / (SPHERE_RESOLUTION - 1);
				p.vertex(graphics->center + geometry::Point{
					graphics->radius * std::cos(a), graphics->radius * std::sin(a)});
			}
			p.fill();
		}
		void draw(const graphics::Circle *g) const override {
			draw_oval(
				g->color,
				g->line_width,
				g->center,
				g->radius, g->radius,
				0, 2 * M_PI);
		}
		void fill(const graphics::Polygon *graphics) const override {
			PathFiller p{mapping, painter, graphics->color};
			for (auto& vertex : graphics->vertices) {
				p.vertex(vertex);
			}
			p.fill();
		}
		void draw(const graphics::Polygon *g) const override {
			auto num_vertices = g->vertices.size();
			for (auto i = 0; i < (int) num_vertices; i++) {
				fill_segment(
					g->color, g->line_width,
					g->vertices[i], g->vertices[(i + 1) % num_vertices],
					true, false
				);
			}
		}
		void draw(const graphics::Lines *graphics) const override {
			for (auto& segment : graphics->segments) {
				fill_segment(
					graphics->color,
					graphics->line_width,
					segment.first,
					segment.second,
					true, true
				);
			}
		}
		void draw(const graphics::Image *graphics) const override {

		}

		void draw(const graphics::Text *g) const override {
			QString font_family{g->font_family.c_str()};
			QFont font{font_family, (int) g->font_size};
			painter.setFont(font);
			QColor color{
				(int) g->color.red,
				(int) g->color.green,
				(int) g->color.blue,
				(int) g->color.alpha};
			painter.setPen(color);
			const auto loc = mapping.map(g->location);

			// TODO: Center it here...
			painter.drawText(loc.x, loc.y, QString::fromStdString(g->text));
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

			PathFiller p{mapping, painter, color};
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
			double begin_angle, double end_angle) const
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

				geometry::Point cur_inner = mapping.map(center + dir - nnormal);
				geometry::Point cur_outer = mapping.map(center + dir + nnormal);


				if (has_prev) {
					PathFiller p{mapping, painter, color};
					p.raw_vertex(prev_inner);
					p.raw_vertex(prev_outer);
					p.raw_vertex(cur_outer);
					p.raw_vertex(cur_inner);
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
