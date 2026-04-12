#include "visualization.hpp"
#include <gtest/gtest.h>

TEST(VisualizationTest, DrawShapesSignature) {
    static_assert(requires(std::span<geometry::Shape> s) { geometry::visualization::Draw(s); });
    SUCCEED();
}

TEST(VisualizationTest, DrawTrianglesSignature) {
    static_assert(
        requires(std::span<const geometry::triangulation::DelaunayTriangle> t) { geometry::visualization::Draw(t); });
    SUCCEED();
}
