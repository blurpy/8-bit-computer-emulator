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
}
