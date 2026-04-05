#include "visualization.hpp"
#include "geometry.hpp"

#include <matplot/matplot.h>
#include <print>

namespace geometry::visualization {

template <class... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};
auto DrawConfig() {
    using namespace geometry;
    using namespace matplot;

    // Disable gnuplot warnings
    auto f = figure(false);
    f->backend()->run_command("unset warnings");
    f->ioff();
    f->size(900, 900);

    hold(on);     // Multiple plots mode
    axis(equal);  // Squre view
    grid(on);     // Enable grid by default
    return f;
}

void Draw(std::span<geometry::Shape> shapes) {
    using namespace geometry;
    using namespace matplot;
    const auto& fh = DrawConfig();
    size_t index = 0;
    for (const auto& shape : shapes) {
        /**
         * @brief Для каждой фигуры примените `std::visit` с помощью мульти-лямбдs (Multilambda),
         *    которая обрабатывает каждый возможный тип фигуры отдельно.
         *    Внутри каждой лямбды:
         *    - Вызовите метод `.Lines()` у фигуры — он возвращает структуру с двумя векторами:
         *      `.x` и `.y`, содержащими координаты точек для отрисовки.
         *    - Передайте эти координаты в функцию `plot(lines.x, lines.y)`.
         *    - Настройте внешний вид линии: установите толщину `.line_width(2)` и задайте цвет `.color()`:
         *        • Line      → "yellow"
         *        • Triangle  → "blue"
         *        • Rectangle → "green"
         *        • RegularPolygon → "magenta"
         *        • Circle    → "red"
         *        • Polygon   → "cyan"
         *
         */

        // ваш код тут
        std::visit(Multilambda{
                       [](const Line& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("yellow");
                       },
                       [](const Triangle& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("blue");
                       },
                       [](const Rectangle& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("green");
                       },
                       [](const RegularPolygon& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("magenta");
                       },
                       [](const Circle& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("red");
                       },
                       [](const Polygon& s) {
                           auto l = s.Lines();
                           plot(l.x, l.y)->line_width(2).color("cyan");
                       },
                   },
                   shape);

        //  Add shape number
        const auto center = std::visit([](const auto& s) { return s.Center(); }, shape);
        auto t = text(center.x, center.y, std::to_string(index));
        t->font_size(14);
        t->color("black");
        ++index;
    }

    // Display plot
    fh->show();
}

void Draw(std::span<const geometry::triangulation::DelaunayTriangle> triangles) {
    using namespace geometry;
    using namespace matplot;

    const auto& fh = DrawConfig();

    size_t index = 0;
    for (const auto& d_triangle : triangles) {
        const geometry::Triangle tri{d_triangle.a, d_triangle.b, d_triangle.c};
        const auto lines = tri.Lines();
        plot(lines.x, lines.y)->line_width(2).color("cyan");

        // Add triangle number
        const auto center = tri.Center();
        auto t = text(center.x, center.y, std::to_string(index));
        t->font_size(14);
        t->color("black");
        ++index;
    }

    // Display plot
    fh->show();
}

}  // namespace geometry::visualization
