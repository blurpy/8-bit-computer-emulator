#include <doctest.h>

#include "core/Utils.h"

using namespace Core;

TEST_SUITE("UtilsTest") {
    TEST_CASE("startsWith() should only return true when parameter is exact and at the start") {
        CHECK(Utils::startsWith("this string", "this"));
        CHECK(Utils::startsWith("this string", "this string"));
        CHECK_FALSE(Utils::startsWith("this string", "string"));

        CHECK(Utils::startsWith(" this string", " this"));
        CHECK_FALSE(Utils::startsWith(" this string", "this"));

        CHECK_FALSE(Utils::startsWith("", "something"));
        CHECK_FALSE(Utils::startsWith("something", ""));

        CHECK_FALSE(Utils::startsWith("something", "something longer"));
    }

    TEST_CASE("isLessThan() should handle constants") {
        CHECK(Utils::isLessThan(0, 1));
        CHECK(Utils::isLessThan(0.1, 0.11));
        CHECK(Utils::isLessThan(0.1, 0.2));
        CHECK(Utils::isLessThan(0.9, 1));
        CHECK(Utils::isLessThan(9.9, 9.91));
        CHECK(Utils::isLessThan(9.991, 9.992));

        CHECK_FALSE(Utils::isLessThan(0, 0));
        CHECK_FALSE(Utils::isLessThan(0.1, 0.1));
        CHECK_FALSE(Utils::isLessThan(0.9, 0.9));
        CHECK_FALSE(Utils::isLessThan(0.91, 0.91));
        CHECK_FALSE(Utils::isLessThan(0.991, 0.991));
        CHECK_FALSE(Utils::isLessThan(1, 1));
        CHECK_FALSE(Utils::isLessThan(1.1, 1.1));
        CHECK_FALSE(Utils::isLessThan(10, 10));

        CHECK_FALSE(Utils::isLessThan(1.1, 1));
        CHECK_FALSE(Utils::isLessThan(2, 1));
        CHECK_FALSE(Utils::isLessThan(10, 1));
        CHECK_FALSE(Utils::isLessThan(10.1, 10));
    }

    TEST_CASE("isLessThan() should handle added decimal numbers") {
        // 0.30000000000000004
        CHECK_FALSE(Utils::isLessThan(0.1 + 0.1 + 0.1, 0.30));
        CHECK(Utils::isLessThan(0.1 + 0.1 + 0.1, 0.31));

        // 0.7999999999999999
        CHECK_FALSE(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.8));
        CHECK(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.81));

        // 0.8999999999999999
        CHECK_FALSE(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.9));
        CHECK(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.91));

        // 0.9999999999999999
        CHECK_FALSE(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1));
        CHECK(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.01));

        // 1.0
        CHECK_FALSE(Utils::isLessThan(0.9 + 0.1, 1));
        CHECK(Utils::isLessThan(0.9 + 0.1, 1.01));

        // 1.0999999999999999
        CHECK_FALSE(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.1));
        CHECK(Utils::isLessThan(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.11));
    }

    TEST_CASE("isLessThan() should handle subtracted decimal numbers") {
        // 1.0999999999999996
        CHECK_FALSE(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1, 1.1));
        CHECK(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1, 1.11));

        // 0.9999999999999997
        CHECK_FALSE(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 1));
        CHECK(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 1.01));

        // 1.0
        CHECK_FALSE(Utils::isLessThan(1.1 - 0.1, 1));
        CHECK(Utils::isLessThan(1.1 - 0.1, 1.01));

        // 0.8999999999999997
        CHECK_FALSE(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.9));
        CHECK(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.91));

        // 0.7999999999999997
        CHECK_FALSE(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.8));
        CHECK(Utils::isLessThan(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.81));

        CHECK_FALSE(Utils::isLessThan(21 - 1, 20));
        CHECK(Utils::isLessThan(20 - 1, 20.01));

        CHECK_FALSE(Utils::isLessThan(0.3 - 0.1, 0.2));
        CHECK_FALSE(Utils::isLessThan(0.3 - 0.1 - 0.1, 0.1));
        CHECK_FALSE(Utils::isLessThan(0.3 - 0.1 - 0.1 - 0.1, 0));

        // 0.09999999999999937
        CHECK_FALSE(Utils::isLessThan(2 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 -
                                      0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.1));
    }

    TEST_CASE("equals() should handle constants") {
        CHECK(Utils::equals(0.3, 0.3));
        CHECK(Utils::equals(0.9, 0.9));
        CHECK(Utils::equals(1, 1));
        CHECK(Utils::equals(1.1, 1.1));
        CHECK(Utils::equals(100, 100));

        CHECK(Utils::equals(0.31, 0.31));
        CHECK(Utils::equals(0.911, 0.911));

        CHECK_FALSE(Utils::equals(0.3, 0.4));
        CHECK_FALSE(Utils::equals(0.4, 0.3));
        CHECK_FALSE(Utils::equals(1, 1.1));
        CHECK_FALSE(Utils::equals(1, 2));

        CHECK_FALSE(Utils::equals(0.31, 0.32));
        CHECK_FALSE(Utils::equals(0.911, 0.912));
    }

    TEST_CASE("equals() should handle added decimal numbers") {
        CHECK(Utils::equals(0 + 0.1, 0.1));
        CHECK(Utils::equals(0.1 + 0.1, 0.2));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1, 0.3));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1, 0.4));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.5));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.6));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.7));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.8));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 0.9));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.1));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.2));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.3));
        CHECK(Utils::equals(0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1, 1.4));
    }

    TEST_CASE("equals() should round subtracted decimal numbers") {
        CHECK(Utils::equals(1.5 - 0.1, 1.4));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1, 1.3));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1, 1.2));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1, 1.1));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 1));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.9));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.8));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.7));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.6));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.5));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.4));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.3));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.2));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0.1));
        CHECK(Utils::equals(1.5 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1 - 0.1, 0));

        CHECK(Utils::equals(0.3 - 0.1, 0.2));
        CHECK(Utils::equals(0.3 - 0.1 - 0.1, 0.1));
        CHECK(Utils::equals(0.3 - 0.1 - 0.1 - 0.1, 0));
    }
}
