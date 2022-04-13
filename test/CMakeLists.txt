# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(vkEngineTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(vkEngine_test source/catch_main.cpp source/vkEngine_test.cpp)
target_link_libraries(vkEngine_test PRIVATE vkEngine_lib Catch2::Catch2)
target_compile_features(vkEngine_test PRIVATE cxx_std_11)

catch_discover_tests(vkEngine_test)

# ---- End-of-file commands ----

add_folders(Test)
