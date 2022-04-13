#include <memory>
#include <string>

#include <catch2/catch.hpp>

extern "C" {
#include "lib.h"
}

TEST_CASE("Name is vkEngine", "[library]")
{
  library lib = create_library();
  auto ptr =
      std::unique_ptr<library, void(*)(library*)>(&lib, &destroy_library);

  REQUIRE(std::string("vkEngine") == lib.name);
}
