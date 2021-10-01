//
// Created by thallock on 9/16/21.
//

#ifndef BILLIARDS_API_PAINT_H
#define BILLIARDS_API_PAINT_H

#include <QPainter>
#include <QPainterPath>

#include "billiards_common/graphics/Circle.h"
#include "billiards_common/graphics/Lines.h"



//	inline
//	void fill(
//		const project::RenderLocation &location,
//		QPainter& painter,
//		const std::shared_ptr<const gphx::Circle>& ptr
//	) {
//	}
//
//
//	inline
//	void fill(
//		const project::RenderLocation &location,
//		QPainter& painter,
//		const std::shared_ptr<const gphx::Polygon>& ptr
//	) {
//		PathFiller p{location, painter, ptr->color};
//		for (int i=0; i<SPHERE_RESOLUTION; i++) {
//			double a = 2 * M_PI * i / (SPHERE_RESOLUTION - 1);
//			p.vertex(ptr->center + geometry::Point{ptr->radius * std::cos(a), ptr->radius * std::sin(a)});
//		}
//		p.fill();
//	}

//
//		template<class iterator>
//		void lines(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			iterator begin, iterator end)
//		{
//			if (begin == end) {
//				return;
//			}
//
//			glBegin(GL_LINES);
//			glColor4d(color.red, color.green, color.blue, color.alpha);
//
//			geometry::Point prv = location.map(*begin);
//			geometry::Point cur;
//
//			++begin;
//			while (begin != end) {
//				cur = location.map(*begin);
//				vertex(prv);
//				vertex(cur);
//				prv = cur;
//				++begin;
//			}
//			glEnd();
//		}
//
//		inline
//		void fill_oval(
//			QPainter& painter,
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			const geometry::Point &center,
//			double rx, double ry,
//			double begin_angle, double end_angle)
//		{
//			PathFiller p{location, painter, color};
//
//			for (int i=0; i<SPHERE_RESOLUTION; i++) {
//				double a = begin_angle + i * (end_angle - begin_angle) / (SPHERE_RESOLUTION - 1);
//				p.vertex(center + geometry::Point{rx * std::cos(a), ry * std::sin(a)});
//			}
////			p.setColor(color);
////			painter.setPen(QColor(255, 0, 0));
////			p.brush.setColor(QColor(500, 0, 0));
//			p.fill();
//		}
//
//		inline
//		void fill_circle(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			const geometry::Point &center,
//			double r)
//		{
//			fill_oval(location, color, center, r, r, 0,
//					  2 * M_PI * (SPHERE_RESOLUTION - 1) / (double) SPHERE_RESOLUTION);
//		}
//
//		template<class iterator>
//		void fill_polygon(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			iterator begin, iterator end)
//		{
//			if (begin == end) {
//				return;
//			}
//
//			glBegin(GL_POLYGON);
//			glColor4d(color.red, color.green, color.blue, color.alpha);
//			do {
//				vertex(location.map(*begin));
//				++begin;
//			} while (begin != end);
//			glEnd();
//		}
//
//		// Doesn't need to draw the last endpoint all the time...
//		inline
//		void fill_line_segment(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			double line_width,
//			const geometry::Point& p1, const geometry::Point& p2
//		) {
//			geometry::Point d = p2 - p1;
//			double dist = d.norm();
//			if (dist < TOLERANCE) {
//				return;
//			}
//			geometry::Point nd = d / dist;
//			geometry::Point orth{nd.y, -nd.x};
//			geometry::Point rorth = orth * line_width;
//
//			std::array<geometry::Point, 4> vertices = {
//				geometry::Point{p1 - rorth},
//				geometry::Point{p2 - rorth},
//				geometry::Point{p2 + rorth},
//				geometry::Point{p1 + rorth}
//			};
//			fill_polygon(location, color, std::begin(vertices), std::end(vertices));
//			fill_circle(location, color, p1, line_width);
//			fill_circle(location, color, p2, line_width);
//		}
//
//
//		template<class iterator>
//		void fill_lines(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			double line_width,
//			iterator begin, iterator end)
//		{
//			if (begin == end) {
//				return;
//			}
//
//			geometry::Point prv = *begin;
//			geometry::Point cur;
//
//			++begin;
//			while (begin != end) {
//				cur = *begin;
//				fill_line_segment(location, color, line_width, prv, cur);
//				prv = cur;
//				++begin;
//			}
//		}
//
//		inline
//		void draw_oval(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			double line_width,
//			const geometry::Point &center,
//			double rx, double ry,
//			double begin_angle, double end_angle)
//		{
//			if (SPHERE_RESOLUTION < 3) {
//				return;
//			}
//
//			geometry::Point prev_outer;
//			geometry::Point prev_inner;
//			bool has_prev = false;
//
//			for (int i=0; i<SPHERE_RESOLUTION; i++) {
//				double a = begin_angle + i * (end_angle - begin_angle) / (SPHERE_RESOLUTION - 1);
//				geometry::Point dir{rx * std::cos(a), ry * std::sin(a)};
//				geometry::Point normal{ry * std::cos(a), rx * std::sin(a)};
//				double nnorm = normal.norm();
//				if (nnorm < TOLERANCE) {
//					has_prev = false;
//					continue;
//				}
//				geometry::Point nnormal = normal * (line_width / nnorm);
//
//				geometry::Point cur_inner = location.map(center + dir - nnormal);
//				geometry::Point cur_outer = location.map(center + dir + nnormal);
//
//				if (has_prev) {
//					glBegin(GL_POLYGON);
//					glColor4d(color.red, color.green, color.blue, color.alpha);
//					vertex(prev_inner);
//					vertex(prev_outer);
//					vertex(cur_outer);
//					vertex(cur_inner);
//					glEnd();
//				}
//
//				prev_inner = cur_inner;
//				prev_outer = cur_outer;
//				has_prev = true;
//			}
//		}
//
//		inline
//		void image(
//			const project::RenderLocation &location,
//			GlImage *img,
//			const geometry::Point &p1, const geometry::Point &p2, const geometry::Point &p3, const geometry::Point &p4)
//		{
//			//	glRasterPos2i(50, 50);
//			//	glDrawPixels(img->width, img->height, GL_RGBA, GL_UNSIGNED_BYTE, img->bytes);
//
//			glEnable(GL_TEXTURE_2D);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//
//			glTexImage2D(GL_TEXTURE_2D, 0, 3,
//						 img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//						 img->bytes);
//
//			geometry::Point mapped;
//			glBegin(GL_POLYGON);
//			{
//				glTexCoord2i(0, 0);
//				vertex(location.map(p1));
//				glTexCoord2i(0, 1);
//				vertex(location.map(p2));
//				glTexCoord2i(1, 1);
//				vertex(location.map(p3));
//				glTexCoord2i(1, 0);
//				vertex(location.map(p4));
//			}
//			glEnd();
//
//			glDisable(GL_TEXTURE_2D);
//		}
//
//		inline
//		void draw_text(
//			const project::RenderLocation &location,
//			const gphx::Color &color,
//			const std::string& str,
//			const geometry::Point &loc)
//		{
//			void *font = GLUT_BITMAP_TIMES_ROMAN_24;
//			//  GLUT_BITMAP_9_BY_15,
//			//  GLUT_BITMAP_TIMES_ROMAN_10,
//			//  GLUT_BITMAP_TIMES_ROMAN_24
//
//			glColor4d(color.red, color.green, color.blue, color.alpha);
//			geometry::Point mapped = location.map(loc);
//			glRasterPos2d(mapped.x, mapped.y);
//			for (char i : str) {
//				glutBitmapCharacter(font, i);
//			}
//		}


//}


#endif //BILLIARDS_API_PAINT_H
