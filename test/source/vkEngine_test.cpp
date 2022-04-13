#include <catch2/catch.hpp>
#include <memory>
#include <string>

extern "C" {
#include "lib.h"
}

TEST_CASE("Name is vkEngine", "[library]") {
	Library lib = CreateLibrary();
	auto ptr = std::unique_ptr<Library, void (*)(Library*)>(&lib, &DestroyLibrary);

	REQUIRE(std::string("vkEngine") == lib.name);
}
