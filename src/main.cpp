#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <print>
#include <ranges>

using namespace geometry;

void PrintAllIntersections(const Shape& shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    /*
     * Используйте ranges чтобы оставить только фигуры,
     * поддерживающие возможность находить пересечения между собой
     *
     * Затем примените монадический интерфейс для обработки результатов:
     *     - Пересечение найдено в точке A между фигурами B и C
     *     - Фигуры B и C не пересекаются
     */

    if (!std::holds_alternative<Line>(shape) && !std::holds_alternative<Circle>(shape)) {
        std::println("Фигура {} не поддерживает поиск пересечений", shape);
        return;
    }

    auto compatible = others | std::views::filter([](const Shape& s) {
                          return std::holds_alternative<Line>(s) || std::holds_alternative<Circle>(s);
                      });

    for (const Shape& other : compatible) {
        auto msg =
            intersections::GetIntersectPoint(shape, other)
                .transform([&](const Point2D& pt) {
                    return std::format("Пересечение найдено в точке {} между фигурами {} и {}", pt, shape, other);
                })
                .value_or(std::format("Фигуры {} и {} не пересекаются", shape, other));
        std::println("{}", msg);
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    /*
     * Используйте ranges чтобы выбрать любые 5 фигур из списка.
     * Затем найдите расстояния от заданной точки до всех выбранных фигур.
     * Выведите результат в формате "Расстояние от точки P до фигуры S равно D"
     */

    for (const Shape& shape : shapes | std::views::take(5)) {
        double dist = queries::DistanceToPoint(shape, p);
        std::println("Расстояние от точки {} до фигуры {} равно {:.4f}", p, shape, dist);
    }
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */

    auto collisions = utils::FindAllCollisions(shapes);
    std::println("Найдено пересечений через BoundingBox: {}", collisions.size());
    for (const auto& [s1, s2] : collisions) {
        std::println("  {} <-> {}", s1, s2);
    }

    if (auto idx = utils::FindHighestShape(shapes)) {
        std::println("Самая высокая фигура (индекс {}): {}", *idx, shapes[*idx]);
    }

    for (size_t i = 0; i < shapes.size(); ++i) {
        for (size_t j = i + 1; j < shapes.size(); ++j) {
            if (auto dist = queries::DistanceBetweenShapes(shapes[i], shapes[j])) {
                std::println("Расстояние между {} и {} равно {:.4f}", shapes[i], shapes[j], *dist);
            }
        }
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Вывести 3 любые фигуры, которые находятся выше 50.0
     *     - Вывести фигуры с наименьшей и с наибольшей высотами
     */

    std::println("Фигуры выше 50.0:");
    for (const Shape& s : shapes | std::views::filter([](const Shape& s) { return queries::GetHeight(s) > 50.0; }) |
                              std::views::take(3)) {
        std::println("  {}", s);
    }

    if (shapes.empty())
        return;

    auto [min_it, max_it] =
        std::ranges::minmax_element(shapes, {}, [](const Shape& s) { return queries::GetHeight(s); });
    std::println("Фигура с наименьшей высотой: {}", *min_it);
    std::println("Фигура с наибольшей высотой: {}", *max_it);
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    for (size_t i = 0; i < shapes.size(); ++i) {
        std::println("Shape {}: height = {:.4f}", i, queries::GetHeight(shapes[i]));
    }

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    /* ваш код здесь */
    for (const Shape& s : shapes) {
        std::visit(
            [&points](const auto& shape) {
                auto verts = shape.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            },
            s);
    }

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    /* ваш код здесь */
    if (auto hull = convex_hull::GrahamScan(points)) {
        shapes.push_back(Polygon{std::move(*hull)});
    } else {
        std::println("Convex hull: недостаточно точек");
    }
    geometry::visualization::Draw(shapes);

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //
        if (auto result = triangulation::DelaunayTriangulation(points)) {
            std::println("Триангуляция Делоне: {} треугольников", result->size());
            for (const auto& t : *result) {
                std::println("  {}", t);
            }
            geometry::visualization::Draw(*result);
        } else {
            std::println("Триангуляция: недостаточно точек");
        }
    }
    return 0;
}
