#include "intersections.hpp"
#include <gtest/gtest.h>

TEST(IntersectionTest, LineLine_Perpendicular) {
    geometry::Line h{{0.0, 1.0}, {2.0, 1.0}};
    geometry::Line v{{1.0, 0.0}, {1.0, 2.0}};
    auto result = geometry::intersections::GetIntersectPoint(h, v);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 1.0, 1e-9);
    EXPECT_NEAR(result->y, 1.0, 1e-9);
}

TEST(IntersectionTest, LineLine_Parallel_NoIntersection) {
    geometry::Line a{{0.0, 0.0}, {2.0, 0.0}};
    geometry::Line b{{0.0, 1.0}, {2.0, 1.0}};
    auto result = geometry::intersections::GetIntersectPoint(a, b);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineLine_NoOverlap) {
    geometry::Line a{{0.0, 0.0}, {1.0, 0.0}};
    geometry::Line b{{2.0, 0.0}, {3.0, 0.0}};
    auto result = geometry::intersections::GetIntersectPoint(a, b);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineCircle_Intersects) {
    geometry::Line l{{-2.0, 0.0}, {2.0, 0.0}};
    geometry::Circle c{{0.0, 0.0}, 1.0};
    auto result = geometry::intersections::GetIntersectPoint(l, c);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->y, 0.0, 1e-9);
}

TEST(IntersectionTest, CircleLine_Intersects) {
    geometry::Circle c{{0.0, 0.0}, 1.0};
    geometry::Line l{{-2.0, 0.0}, {2.0, 0.0}};
    auto result = geometry::intersections::GetIntersectPoint(c, l);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionTest, LineCircle_NoIntersection) {
    geometry::Line l{{0.0, 2.0}, {2.0, 2.0}};
    geometry::Circle c{{0.0, 0.0}, 1.0};
    auto result = geometry::intersections::GetIntersectPoint(l, c);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, CircleCircle_Overlapping) {
    geometry::Circle c1{{0.0, 0.0}, 1.0};
    geometry::Circle c2{{1.0, 0.0}, 1.0};
    auto result = geometry::intersections::GetIntersectPoint(c1, c2);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionTest, CircleCircle_Separate) {
    geometry::Circle c1{{0.0, 0.0}, 1.0};
    geometry::Circle c2{{5.0, 0.0}, 1.0};
    auto result = geometry::intersections::GetIntersectPoint(c1, c2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, CircleCircle_Tangent) {
    geometry::Circle c1{{0.0, 0.0}, 1.0};
    geometry::Circle c2{{2.0, 0.0}, 1.0};
    auto result = geometry::intersections::GetIntersectPoint(c1, c2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 1.0, 1e-9);
    EXPECT_NEAR(result->y, 0.0, 1e-9);
}

TEST(IntersectionTest, UnsupportedPair_Throws) {
    geometry::Shape t1 = geometry::Triangle{{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}};
    geometry::Shape t2 = geometry::Triangle{{2.0, 0.0}, {3.0, 0.0}, {2.5, 1.0}};
    EXPECT_THROW(geometry::intersections::GetIntersectPoint(t1, t2), std::logic_error);
}
