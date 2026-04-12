#include "convex_hull.hpp"
#include <algorithm>
#include <gtest/gtest.h>

TEST(GrahamScanTest, TooFewPointsZero) {
    std::vector<geometry::Point2D> pts;
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), geometry::convex_hull::GrahamScanError::TooFewPoints);
}

TEST(GrahamScanTest, TooFewPointsTwo) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {1.0, 1.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), geometry::convex_hull::GrahamScanError::TooFewPoints);
}

TEST(GrahamScanTest, Triangle) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3u);
}

TEST(GrahamScanTest, SquareAllOnHull) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4u);
}

TEST(GrahamScanTest, PivotNotInitiallyFirst) {
    std::vector<geometry::Point2D> pts = {{4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}, {0.0, 0.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4u);
}

TEST(GrahamScanTest, InteriorPointExcluded) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}, {2.0, 2.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4u);
}

TEST(GrahamScanTest, HullPointsAreSubsetOfInput) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {6.0, 0.0}, {6.0, 6.0}, {0.0, 6.0}, {1.0, 1.0}, {3.0, 3.0}};
    auto result = geometry::convex_hull::GrahamScan(pts);
    ASSERT_TRUE(result.has_value());

    for (const auto& hp : *result) {
        bool found = std::ranges::any_of(pts, [&](const geometry::Point2D& p) {
            return std::abs(p.x - hp.x) < 1e-9 && std::abs(p.y - hp.y) < 1e-9;
        });
        EXPECT_TRUE(found) << "Hull point (" << hp.x << ", " << hp.y << ") not in input";
    }
}

TEST(GrahamScanTest, IsNoexcept) {
    std::vector<geometry::Point2D> pts = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    std::span<geometry::Point2D> s = pts;
    static_assert(noexcept(geometry::convex_hull::GrahamScan(s)));
}
