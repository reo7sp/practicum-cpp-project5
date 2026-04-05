#include "shape_utils.hpp"
#include <gtest/gtest.h>
#include <variant>

TEST(ParseShapesTest, Circle) {
    auto shapes = geometry::utils::ParseShapes("circle 1 2 3");
    ASSERT_EQ(shapes.size(), 1u);
    ASSERT_TRUE(std::holds_alternative<geometry::Circle>(shapes[0]));
    auto& c = std::get<geometry::Circle>(shapes[0]);
    EXPECT_DOUBLE_EQ(c.center_p.x, 1.0);
    EXPECT_DOUBLE_EQ(c.center_p.y, 2.0);
    EXPECT_DOUBLE_EQ(c.radius, 3.0);
}

TEST(ParseShapesTest, Line) {
    auto shapes = geometry::utils::ParseShapes("line 0 0 4 4");
    ASSERT_EQ(shapes.size(), 1u);
    ASSERT_TRUE(std::holds_alternative<geometry::Line>(shapes[0]));
    auto& l = std::get<geometry::Line>(shapes[0]);
    EXPECT_DOUBLE_EQ(l.start.x, 0.0);
    EXPECT_DOUBLE_EQ(l.end.x, 4.0);
}

TEST(ParseShapesTest, Triangle) {
    auto shapes = geometry::utils::ParseShapes("triangle 0 0 1 0 0.5 1");
    ASSERT_EQ(shapes.size(), 1u);
    ASSERT_TRUE(std::holds_alternative<geometry::Triangle>(shapes[0]));
}

TEST(ParseShapesTest, Rectangle) {
    auto shapes = geometry::utils::ParseShapes("rectangle 1 2 3 4");
    ASSERT_EQ(shapes.size(), 1u);
    ASSERT_TRUE(std::holds_alternative<geometry::Rectangle>(shapes[0]));
    auto& r = std::get<geometry::Rectangle>(shapes[0]);
    EXPECT_DOUBLE_EQ(r.width, 3.0);
    EXPECT_DOUBLE_EQ(r.height, 4.0);
}

TEST(ParseShapesTest, RegularPolygon) {
    auto shapes = geometry::utils::ParseShapes("polygon 0 0 5 6");
    ASSERT_EQ(shapes.size(), 1u);
    ASSERT_TRUE(std::holds_alternative<geometry::RegularPolygon>(shapes[0]));
    auto& p = std::get<geometry::RegularPolygon>(shapes[0]);
    EXPECT_DOUBLE_EQ(p.radius, 5.0);
    EXPECT_EQ(p.sides, 6);
}

TEST(ParseShapesTest, BadShapeSkipped) {
    auto shapes = geometry::utils::ParseShapes("badshape 1 2 3");
    EXPECT_EQ(shapes.size(), 0u);
}

TEST(ParseShapesTest, NegativeRadiusSkipped) {
    auto shapes = geometry::utils::ParseShapes("circle 0 0 -1");
    EXPECT_EQ(shapes.size(), 0u);
}

TEST(ParseShapesTest, MultipleShapesSemicolon) {
    auto shapes = geometry::utils::ParseShapes("circle 0 0 1; line 0 0 1 1; badshape");
    EXPECT_EQ(shapes.size(), 2u);
}

TEST(ParseShapesTest, PolygonTooFewSidesSkipped) {
    auto shapes = geometry::utils::ParseShapes("polygon 0 0 5 2");
    EXPECT_EQ(shapes.size(), 0u);
}

TEST(FindHighestShapeTest, Empty) {
    std::vector<geometry::Shape> shapes;
    EXPECT_FALSE(geometry::utils::FindHighestShape(shapes).has_value());
}

TEST(FindHighestShapeTest, SingleShape) {
    std::vector<geometry::Shape> shapes = {geometry::Circle{{0.0, 0.0}, 1.0}};
    auto idx = geometry::utils::FindHighestShape(shapes);
    ASSERT_TRUE(idx.has_value());
    EXPECT_EQ(*idx, 0u);
}

TEST(FindHighestShapeTest, FindsHighest) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 1.0},
        geometry::Circle{{0.0, 5.0}, 1.0},
        geometry::Circle{{0.0, 2.0}, 1.0},
    };
    auto idx = geometry::utils::FindHighestShape(shapes);
    ASSERT_TRUE(idx.has_value());
    EXPECT_EQ(*idx, 1u);
}

TEST(FindAllCollisionsTest, NoCollisions) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 1.0},
        geometry::Circle{{10.0, 0.0}, 1.0},
    };
    auto collisions = geometry::utils::FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0u);
}

TEST(FindAllCollisionsTest, OneCollision) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 2.0},
        geometry::Circle{{2.0, 0.0}, 2.0},
        geometry::Circle{{20.0, 0.0}, 1.0},
    };
    auto collisions = geometry::utils::FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 1u);
}

TEST(FindAllCollisionsTest, AllOverlap) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 5.0},
        geometry::Circle{{1.0, 0.0}, 5.0},
        geometry::Circle{{0.0, 1.0}, 5.0},
    };
    auto collisions = geometry::utils::FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 3u);
}
