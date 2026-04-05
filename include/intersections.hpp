#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>
#include <stdexcept>
#include <variant>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    /* ваш код здесь */
    std::optional<Point2D> operator()(const Line& x, const Line& y) const { return intersectLineLine(x, y); }

    std::optional<Point2D> operator()(const Line& x, const Circle& y) const { return intersectLineCircle(x, y); }

    std::optional<Point2D> operator()(const Circle& x, const Line& y) const { return intersectLineCircle(y, x); }

    std::optional<Point2D> operator()(const Circle& x, const Circle& y) const { return intersectCircleCircle(x, y); }

    template <typename T, typename U>
    std::optional<Point2D> operator()(const T&, const U&) const {
        throw std::logic_error("unsupported shape pair for intersection");
    }

private:
    static constexpr double kEpsilon = 1e-9;

    static double crossProduct(Point2D u, Point2D v) { return u.x * v.y - u.y * v.x; }

    static double dotProduct(Point2D u, Point2D v) { return u.x * v.x + u.y * v.y; }

    static std::optional<Point2D> intersectLineLine(const Line& p, const Line& q) {
        Point2D r{p.end.x - p.start.x, p.end.y - p.start.y};
        Point2D s{q.end.x - q.start.x, q.end.y - q.start.y};
        double denominator = crossProduct(r, s);
        if (std::abs(denominator) < kEpsilon) {
            return std::nullopt;
        }

        Point2D w{q.start.x - p.start.x, q.start.y - p.start.y};
        double t = crossProduct(w, s) / denominator;
        double u = crossProduct(w, r) / denominator;
        if (t < -kEpsilon || t > 1 + kEpsilon || u < -kEpsilon || u > 1 + kEpsilon) {
            return std::nullopt;
        }

        return Point2D{p.start.x + t * r.x, p.start.y + t * r.y};
    }

    static std::optional<Point2D> intersectLineCircle(const Line& line, const Circle& circle) {
        Point2D d{line.end.x - line.start.x, line.end.y - line.start.y};
        double a = dotProduct(d, d);
        if (a < kEpsilon) {
            return std::nullopt;
        }

        Point2D f{line.start.x - circle.center_p.x, line.start.y - circle.center_p.y};
        double b = 2 * dotProduct(f, d);
        double c = dotProduct(f, f) - circle.radius * circle.radius;
        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return std::nullopt;
        }

        double sqrtDisc = std::sqrt(discriminant);
        double t0 = (-b - sqrtDisc) / (2 * a);
        double t1 = (-b + sqrtDisc) / (2 * a);
        auto tInSegment = [](double t) { return t >= -kEpsilon && t <= 1 + kEpsilon; };
        bool hasT0 = tInSegment(t0), hasT1 = tInSegment(t1);
        if (!hasT0 && !hasT1) {
            return std::nullopt;
        }

        double t = (hasT0 && hasT1) ? std::min(t0, t1) : (hasT0 ? t0 : t1);

        return Point2D{line.start.x + t * d.x, line.start.y + t * d.y};
    }

    static std::optional<Point2D> intersectCircleCircle(const Circle& c0, const Circle& c1) {
        double dx = c1.center_p.x - c0.center_p.x;
        double dy = c1.center_p.y - c0.center_p.y;
        double dist = std::hypot(dx, dy);
        if (dist < kEpsilon || dist > c0.radius + c1.radius + kEpsilon ||
            dist < std::abs(c0.radius - c1.radius) - kEpsilon) {
            return std::nullopt;
        }

        double nx = dx / dist;
        double ny = dy / dist;
        double along = (c0.radius * c0.radius - c1.radius * c1.radius + dist * dist) / (2 * dist);
        double hSquared = c0.radius * c0.radius - along * along;
        if (hSquared < 0) {
            return std::nullopt;
        }
        if (hSquared <= kEpsilon) {
            return Point2D{c0.center_p.x + along * nx, c0.center_p.y + along * ny};
        }

        double h = std::sqrt(hSquared);

        return Point2D{c0.center_p.x + along * nx - ny * h, c0.center_p.y + along * ny + nx * h};
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape& shape1, const Shape& shape2) {
    /* ваш код здесь */
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections
