#include "geometry.hpp"
#include <cmath>
#include <gtest/gtest.h>

TEST(Point2DTest, DefaultConstructor) {
    geometry::Point2D p;
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}

TEST(Point2DTest, Arithmetic) {
    geometry::Point2D a{1.0, 2.0}, b{3.0, 4.0};
    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum.x, 4.0);
    EXPECT_DOUBLE_EQ(sum.y, 6.0);

    auto diff = b - a;
    EXPECT_DOUBLE_EQ(diff.x, 2.0);
    EXPECT_DOUBLE_EQ(diff.y, 2.0);

    auto scaled = a * 3.0;
    EXPECT_DOUBLE_EQ(scaled.x, 3.0);
    EXPECT_DOUBLE_EQ(scaled.y, 6.0);

    auto divided = b / 2.0;
    EXPECT_DOUBLE_EQ(divided.x, 1.5);
    EXPECT_DOUBLE_EQ(divided.y, 2.0);
}

TEST(Point2DTest, EqualityOperator) {
    EXPECT_TRUE((geometry::Point2D{1.0, 2.0} == geometry::Point2D{1.0, 2.0}));
    EXPECT_FALSE((geometry::Point2D{1.0, 2.0} == geometry::Point2D{1.0, 3.0}));
}

TEST(Point2DTest, DotProduct) {
    geometry::Point2D a{1.0, 0.0}, b{0.0, 1.0};
    EXPECT_DOUBLE_EQ(a.Dot(b), 0.0);
    EXPECT_DOUBLE_EQ(a.Dot(a), 1.0);
    EXPECT_DOUBLE_EQ((geometry::Point2D{2.0, 3.0}.Dot(geometry::Point2D{4.0, 5.0})), 23.0);
}

TEST(Point2DTest, CrossProduct) {
    geometry::Point2D a{1.0, 0.0}, b{0.0, 1.0};
    EXPECT_DOUBLE_EQ(a.Cross(b), 1.0);
    EXPECT_DOUBLE_EQ(b.Cross(a), -1.0);
    EXPECT_DOUBLE_EQ(a.Cross(a), 0.0);
}

TEST(Point2DTest, Length) {
    geometry::Point2D a{3.0, 4.0}, z{0.0, 0.0};
    EXPECT_DOUBLE_EQ(a.Length(), 5.0);
    EXPECT_DOUBLE_EQ(z.Length(), 0.0);
}

TEST(Point2DTest, DistanceTo) {
    geometry::Point2D origin{0.0, 0.0}, p{3.0, 4.0}, q{1.0, 1.0};
    EXPECT_DOUBLE_EQ(origin.DistanceTo(p), 5.0);
    EXPECT_DOUBLE_EQ(q.DistanceTo(q), 0.0);
}

TEST(Point2DTest, Normalize) {
    auto n = geometry::Point2D{3.0, 4.0}.Normalize();
    EXPECT_NEAR(n.x, 0.6, 1e-9);
    EXPECT_NEAR(n.y, 0.8, 1e-9);

    auto zero = geometry::Point2D{0.0, 0.0}.Normalize();
    EXPECT_DOUBLE_EQ(zero.x, 0.0);
    EXPECT_DOUBLE_EQ(zero.y, 0.0);
}

TEST(BoundingBoxTest, WidthHeight) {
    geometry::BoundingBox bb{1.0, 2.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(bb.Width(), 4.0);
    EXPECT_DOUBLE_EQ(bb.Height(), 4.0);
}

TEST(BoundingBoxTest, Center) {
    geometry::BoundingBox bb{0.0, 0.0, 4.0, 2.0};
    auto c = bb.Center();
    EXPECT_DOUBLE_EQ(c.x, 2.0);
    EXPECT_DOUBLE_EQ(c.y, 1.0);
}

TEST(BoundingBoxTest, Overlaps) {
    geometry::BoundingBox a{0.0, 0.0, 2.0, 2.0};
    geometry::BoundingBox b{1.0, 1.0, 3.0, 3.0};
    geometry::BoundingBox c{5.0, 5.0, 7.0, 7.0};

    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_TRUE(b.Overlaps(a));
    EXPECT_FALSE(a.Overlaps(c));
    EXPECT_FALSE(c.Overlaps(a));
}

TEST(BoundingBoxTest, TouchingEdgesOverlap) {
    geometry::BoundingBox a{0.0, 0.0, 1.0, 1.0};
    geometry::BoundingBox b{1.0, 0.0, 2.0, 1.0};
    EXPECT_TRUE(a.Overlaps(b));
}

TEST(LineTest, BoundBox) {
    geometry::Line l{{0.0, 1.0}, {4.0, 3.0}};
    auto bb = l.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 0.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 1.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 3.0);
}

TEST(TriangleTest, Area) {
    geometry::Triangle t{{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}};
    EXPECT_DOUBLE_EQ(t.Area(), 6.0);
}

TEST(CircleTest, BoundBox) {
    geometry::Circle c{{1.0, 2.0}, 3.0};
    auto bb = c.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, -2.0);
    EXPECT_DOUBLE_EQ(bb.min_y, -1.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 5.0);
}

TEST(RectangleTest, BoundBox) {
    geometry::Rectangle r{{1.0, 2.0}, 3.0, 4.0};
    auto bb = r.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 1.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 6.0);
}

TEST(RegularPolygonTest, VertexCount) {
    geometry::RegularPolygon p{{0.0, 0.0}, 5.0, 6};
    EXPECT_EQ(p.Vertices().size(), 6u);
}

TEST(RegularPolygonTest, BoundBox) {
    geometry::RegularPolygon p{{0.0, 0.0}, 3.0, 4};
    auto bb = p.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, -3.0);
    EXPECT_DOUBLE_EQ(bb.min_y, -3.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 3.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 3.0);
}

TEST(RegularPolygonTest, Height) {
    geometry::RegularPolygon p{{0.0, 2.0}, 3.0, 5};
    EXPECT_DOUBLE_EQ(p.Height(), 5.0);
}

TEST(PolygonTest, BoundBox) {
    geometry::Polygon p{{{0.0, 0.0}, {4.0, 0.0}, {4.0, 3.0}, {0.0, 3.0}}};
    auto bb = p.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 0.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 0.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 3.0);
}

TEST(PolygonTest, VertexCount) {
    geometry::Polygon p{{{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}, {0.0, 0.5}}};
    EXPECT_EQ(p.Vertices().size(), 5u);
}

TEST(PolygonTest, Center) {
    geometry::Polygon p{{{0.0, 0.0}, {4.0, 0.0}, {4.0, 2.0}, {0.0, 2.0}}};
    auto c = p.Center();
    EXPECT_DOUBLE_EQ(c.x, 2.0);
    EXPECT_DOUBLE_EQ(c.y, 1.0);
}
