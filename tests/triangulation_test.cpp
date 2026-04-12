#include "triangulation.hpp"
#include <algorithm>
#include <gtest/gtest.h>

TEST(DelaunayTriangulationTest, TooFewPointsZero) {
    std::vector<geometry::Point2D> pts;
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), geometry::triangulation::DelaunayTriangulationError::TooFewPoints);
}

TEST(DelaunayTriangulationTest, TooFewPointsTwo) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {1.0, 1.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), geometry::triangulation::DelaunayTriangulationError::TooFewPoints);
}

TEST(DelaunayTriangulationTest, ThreePoints_OneTriangle) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 1u);
}

TEST(DelaunayTriangulationTest, FourPoints_TwoTriangles) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2u);
}

TEST(DelaunayTriangulationTest, FivePoints_TriangleCount) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {10.0, 0.0}, {5.0, 8.0}, {15.0, 5.0}, {2.0, 12.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_GT(result->size(), 0u);
}

TEST(DelaunayTriangulationTest, AllTriangleVerticesFromInput) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {6.0, 0.0}, {6.0, 6.0}, {0.0, 6.0}, {3.0, 3.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(pts);
    ASSERT_TRUE(result.has_value());

    auto in_input = [&](const geometry::Point2D& p) {
        return std::ranges::any_of(
            pts, [&](const geometry::Point2D& q) { return std::abs(p.x - q.x) < 1e-9 && std::abs(p.y - q.y) < 1e-9; });
    };

    for (const auto& t : *result) {
        EXPECT_TRUE(in_input(t.a));
        EXPECT_TRUE(in_input(t.b));
        EXPECT_TRUE(in_input(t.c));
    }
}

TEST(DelaunayTriangulationTest, IsNoexcept) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    std::span<const geometry::Point2D> s = pts;
    static_assert(noexcept(geometry::triangulation::DelaunayTriangulation(s)));
}
