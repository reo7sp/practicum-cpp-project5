#include "queries.hpp"
#include <gtest/gtest.h>

TEST(DistanceToPointTest, Line_PointOnLine) {
    geometry::Shape s = geometry::Line{{0.0, 0.0}, {4.0, 0.0}};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {2.0, 0.0}), 0.0, 1e-9);
}

TEST(DistanceToPointTest, Line_PointAbove) {
    geometry::Shape s = geometry::Line{{0.0, 0.0}, {4.0, 0.0}};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {2.0, 3.0}), 3.0, 1e-9);
}

TEST(DistanceToPointTest, Line_PointBeyondEnd) {
    geometry::Shape s = geometry::Line{{0.0, 0.0}, {4.0, 0.0}};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {6.0, 0.0}), 2.0, 1e-9);
}

TEST(DistanceToPointTest, Circle_Outside) {
    geometry::Shape s = geometry::Circle{{0.0, 0.0}, 1.0};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {3.0, 0.0}), 2.0, 1e-9);
}

TEST(DistanceToPointTest, Circle_Inside) {
    geometry::Shape s = geometry::Circle{{0.0, 0.0}, 5.0};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {0.0, 0.0}), 0.0, 1e-9);
}

TEST(DistanceToPointTest, Triangle) {
    geometry::Shape s = geometry::Triangle{{0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0}};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {2.0, -3.0}), 3.0, 1e-9);
}

TEST(DistanceToPointTest, Rectangle) {
    geometry::Shape s = geometry::Rectangle{{0.0, 0.0}, 4.0, 2.0};
    EXPECT_NEAR(geometry::queries::DistanceToPoint(s, {2.0, -1.0}), 1.0, 1e-9);
}

TEST(GetHeightTest, Line) {
    geometry::Shape s = geometry::Line{{1.0, 2.0}, {3.0, 5.0}};
    EXPECT_DOUBLE_EQ(geometry::queries::GetHeight(s), 5.0);
}

TEST(GetHeightTest, Circle) {
    geometry::Shape s = geometry::Circle{{0.0, 3.0}, 2.0};
    EXPECT_DOUBLE_EQ(geometry::queries::GetHeight(s), 5.0);
}

TEST(GetHeightTest, Triangle) {
    geometry::Shape s = geometry::Triangle{{0.0, 1.0}, {2.0, 0.0}, {1.0, 4.0}};
    EXPECT_DOUBLE_EQ(geometry::queries::GetHeight(s), 4.0);
}

TEST(GetHeightTest, Rectangle) {
    geometry::Shape s = geometry::Rectangle{{0.0, 1.0}, 3.0, 5.0};
    EXPECT_DOUBLE_EQ(geometry::queries::GetHeight(s), 6.0);
}

TEST(GetBoundBoxTest, Circle) {
    geometry::Shape s = geometry::Circle{{2.0, 3.0}, 1.0};
    auto bb = geometry::queries::GetBoundBox(s);
    EXPECT_DOUBLE_EQ(bb.min_x, 1.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 3.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 4.0);
}

TEST(BoundingBoxesOverlapTest, Overlapping) {
    geometry::Shape s1 = geometry::Circle{{0.0, 0.0}, 2.0};
    geometry::Shape s2 = geometry::Circle{{1.0, 0.0}, 2.0};
    EXPECT_TRUE(geometry::queries::BoundingBoxesOverlap(s1, s2));
}

TEST(BoundingBoxesOverlapTest, NotOverlapping) {
    geometry::Shape s1 = geometry::Circle{{0.0, 0.0}, 1.0};
    geometry::Shape s2 = geometry::Circle{{10.0, 0.0}, 1.0};
    EXPECT_FALSE(geometry::queries::BoundingBoxesOverlap(s1, s2));
}

TEST(DistanceBetweenShapesTest, CircleCircle_Separated) {
    geometry::Shape s1 = geometry::Circle{{0.0, 0.0}, 1.0};
    geometry::Shape s2 = geometry::Circle{{5.0, 0.0}, 1.0};
    auto d = geometry::queries::DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_NEAR(*d, 3.0, 1e-9);
}

TEST(DistanceBetweenShapesTest, CircleCircle_Overlapping) {
    geometry::Shape s1 = geometry::Circle{{0.0, 0.0}, 2.0};
    geometry::Shape s2 = geometry::Circle{{1.0, 0.0}, 2.0};
    auto d = geometry::queries::DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_DOUBLE_EQ(*d, 0.0);
}

TEST(DistanceBetweenShapesTest, LineLine) {
    geometry::Shape s1 = geometry::Line{{0.0, 0.0}, {1.0, 0.0}};
    geometry::Shape s2 = geometry::Line{{0.0, 3.0}, {1.0, 3.0}};
    auto d = geometry::queries::DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_NEAR(*d, 3.0, 1e-9);
}

TEST(DistanceBetweenShapesTest, UnsupportedPair_ReturnsNullopt) {
    geometry::Shape s1 = geometry::Triangle{{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}};
    geometry::Shape s2 = geometry::Circle{{5.0, 5.0}, 1.0};
    auto d = geometry::queries::DistanceBetweenShapes(s1, s2);
    EXPECT_FALSE(d.has_value());
}
