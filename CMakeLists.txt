cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    vkEngine
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    vkEngine_lib OBJECT
    source/lib.c
)

target_include_directories(
    vkEngine_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(vkEngine_lib PUBLIC c_std_11)

find_package(json-c REQUIRED)
target_link_libraries(vkEngine_lib PRIVATE json-c::json-c)

find_path(HEDLEY_INCLUDE_DIR hedley.h)
if(NOT HEDLEY_INCLUDE_DIR)
  message(FATAL_ERROR "Couldn't find hedley.h (${HEDLEY_INCLUDE_DIR})")
endif()
target_include_directories(
    vkEngine_lib SYSTEM
    PRIVATE "${HEDLEY_INCLUDE_DIR}"
)

# ---- Declare executable ----

add_executable(vkEngine_exe source/main.c)
add_executable(vkEngine::exe ALIAS vkEngine_exe)

set_target_properties(
    vkEngine_exe PROPERTIES
    OUTPUT_NAME vkEngine
    EXPORT_NAME exe
)

target_compile_features(vkEngine_exe PRIVATE c_std_11)

target_link_libraries(vkEngine_exe PRIVATE vkEngine_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT vkEngine_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of vkEngine"
  )
endif()

include(cmake/dev-mode.cmake)
