#include "geometry/vector_2d.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

using Catch::Approx;
using JamJar::Vector2D;

Catch::Approx ApproxZero() { return Approx(0).margin(std::numeric_limits<float>::epsilon() * 100); }

TEST_CASE("Vector2D constructors", "[Vector2D]") {
    SECTION("Default constructor") {
        auto defaultConstructed = Vector2D();
        REQUIRE(defaultConstructed.x == Approx(0));
        REQUIRE(defaultConstructed.y == Approx(0));
    }

    SECTION("Value constructor") {
        auto a = Vector2D(3, 5);
        REQUIRE(a.x == Approx(3));
        REQUIRE(a.y == Approx(5));
    }

    SECTION("Copy constructor") {
        auto a = Vector2D(3, 5);
        auto b = a;
        a.x = 100;
        a.y = 100;
        REQUIRE(b.x == Approx(3));
        REQUIRE(b.y == Approx(5));
    }
}

TEST_CASE("Vector2D addition", "[Vector2D]") {
    SECTION("+") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        auto c = a + b;
        REQUIRE(c.x == Approx(105));
        REQUIRE(c.y == Approx(108));
    }

    SECTION("+=") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        a += b;
        REQUIRE(a.x == Approx(105));
        REQUIRE(a.y == Approx(108));
    }
}

TEST_CASE("Vector2D subtraction", "[Vector2D]") {
    SECTION("+") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        auto c = a - b;
        REQUIRE(c.x == Approx(95));
        REQUIRE(c.y == Approx(92));
    }

    SECTION("+=") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        a -= b;
        REQUIRE(a.x == Approx(95));
        REQUIRE(a.y == Approx(92));
    }
}

TEST_CASE("Vector2D multiplication", "[Vector2D]") {
    SECTION("+") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        auto c = a * b;
        REQUIRE(c.x == Approx(500));
        REQUIRE(c.y == Approx(800));
    }

    SECTION("+=") {
        auto a = Vector2D(100, 100);
        auto b = Vector2D(5, 8);
        a *= b;
        REQUIRE(a.x == Approx(500));
        REQUIRE(a.y == Approx(800));
    }
}

TEST_CASE("Vector2D scale", "[Vector2D]") {
    SECTION("+") {
        auto a = Vector2D(100, 100);
        auto b = 5;
        auto c = a * b;
        REQUIRE(c.x == Approx(500));
        REQUIRE(c.y == Approx(500));
    }

    SECTION("+=") {
        auto a = Vector2D(100, 100);
        auto b = 5;
        a *= b;
        REQUIRE(a.x == Approx(500));
        REQUIRE(a.y == Approx(500));
    }
}

TEST_CASE("Vector2D dot", "[Vector2D]") {
    SECTION("(10, 10) . (10, 10)") {
        auto a = Vector2D(10, 10);
        auto b = Vector2D(10, 10);
        REQUIRE(a.Dot(b) == Approx(200));
    }
}

TEST_CASE("Vector2D magnitude", "[Vector2D]") {
    SECTION("(4,2)") {
        auto a = Vector2D(4, 2);
        REQUIRE(a.Magnitude() == Approx(4.47214f));
    }
}

TEST_CASE("Vector2D rotate", "[Vector2D]") {
    SECTION("Rotate 90 degrees from diagonal") {
        auto vec = Vector2D(10, 10);
        auto center = Vector2D(0, 0);
        auto angle = M_PI / 2;
        auto result = vec.Rotate(center, angle);
        REQUIRE(result.x == Approx(-10));
        REQUIRE(result.y == Approx(10));
    }

    SECTION("Rotate 90 degrees from vertical") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = M_PI / 2;
        auto result = vec.Rotate(center, angle);
        REQUIRE(result.x == Approx(-10));
        REQUIRE(result.y == ApproxZero());
    }

    SECTION("Rotate 180 degrees from vertical") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = M_PI;
        auto result = vec.Rotate(center, angle);
        REQUIRE(result.x == ApproxZero());
        REQUIRE(result.y == Approx(-10));
    }

    SECTION("Rotate -45 degrees from horizontal") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = -M_PI / 4;
        auto result = vec.Rotate(center, angle);
        REQUIRE(result.x == Approx(7.071));
        REQUIRE(result.y == Approx(7.071));
    }
}

TEST_CASE("Vector2D rotate in place", "[Vector2D]") {
    SECTION("Rotate 90 degrees from diagonal") {
        auto vec = Vector2D(10, 10);
        auto center = Vector2D(0, 0);
        auto angle = M_PI / 2;
        vec.RotateInPlace(center, angle);
        REQUIRE(vec.x == Approx(-10));
        REQUIRE(vec.y == Approx(10));
    }
}

TEST_CASE("Vector2D rotate degrees", "[Vector2D]") {
    SECTION("Rotate 90 degrees from diagonal") {
        auto vec = Vector2D(10, 10);
        auto center = Vector2D(0, 0);
        auto angle = 90;
        auto result = vec.RotateDeg(center, angle);
        REQUIRE(result.x == Approx(-10));
        REQUIRE(result.y == Approx(10));
    }

    SECTION("Rotate 90 degrees from vertical") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = 90;
        auto result = vec.RotateDeg(center, angle);
        REQUIRE(result.x == Approx(-10));
        REQUIRE(result.y == ApproxZero());
    }

    SECTION("Rotate 180 degrees from vertical") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = 180;
        auto result = vec.RotateDeg(center, angle);
        REQUIRE(result.x == ApproxZero());
        REQUIRE(result.y == Approx(-10));
    }

    SECTION("Rotate -45 degrees from horizontal") {
        auto vec = Vector2D(0, 10);
        auto center = Vector2D(0, 0);
        auto angle = -45;
        auto result = vec.RotateDeg(center, angle);
        REQUIRE(result.x == Approx(7.071));
        REQUIRE(result.y == Approx(7.071));
    }
}

TEST_CASE("Vector2D rotate degrees in place", "[Vector2D]") {
    SECTION("Rotate 90 degrees from diagonal") {
        auto vec = Vector2D(10, 10);
        auto center = Vector2D(0, 0);
        auto angle = 90;
        vec.RotateDegInPlace(center, angle);
        REQUIRE(vec.x == Approx(-10));
        REQUIRE(vec.y == Approx(10));
    }
}

TEST_CASE("Vector2D invert", "[Vector2D]") {
    SECTION("(-15, -10)") {
        auto vec = Vector2D(-15, -10);
        auto result = vec.Invert();
        REQUIRE(result.x == Approx(15));
        REQUIRE(result.y == Approx(10));
    }
}

TEST_CASE("Vector2D invert in place", "[Vector2D]") {
    SECTION("(-15, -10)") {
        auto vec = Vector2D(-15, -10);
        vec.InvertInPlace();
        REQUIRE(vec.x == Approx(15));
        REQUIRE(vec.y == Approx(10));
    }
}

TEST_CASE("Vector2D normalize", "[Vector2D]") {
    SECTION("(5, 10)") {
        auto vec = Vector2D(5, 10);
        auto result = vec.Normalize();
        REQUIRE(result.x == Approx(0.44721f));
        REQUIRE(result.y == Approx(0.89443f));
    }

    SECTION("(0.1, 0.2)") {
        auto vec = Vector2D(0.1, 0.2);
        auto result = vec.Normalize();
        REQUIRE(result.x == Approx(0.44721f));
        REQUIRE(result.y == Approx(0.89443f));
    }
}

TEST_CASE("Vector2D normalize in place", "[Vector2D]") {
    SECTION("(5, 10)") {
        auto vec = Vector2D(5, 10);
        vec.NormalizeInPlace();
        REQUIRE(vec.x == Approx(0.44721f));
        REQUIRE(vec.y == Approx(0.89443f));
    }
}

TEST_CASE("Vector2D distance", "[Vector2D]") {
    SECTION("(3, 3) to (6, 6)") {
        auto a = Vector2D(3, 3);
        auto b = Vector2D(6, 6);
        REQUIRE(a.Distance(b) == Approx(4.24264f));
    }
}
