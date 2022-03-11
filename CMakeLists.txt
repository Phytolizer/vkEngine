cmake_minimum_required(VERSION 3.16...3.23)

project(
    "vkEngine"
    LANGUAGES C
    VERSION "0.1.0"
    DESCRIPTION "Vulkan Engine"
    HOMEPAGE_URL "https://github.com/Phytolizer/vkEngine"
)

include(FetchContent)

find_package(glfw3)

if(NOT glfw3_FOUND)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.3.6"
    )
    FetchContent_MakeAvailable(glfw)
endif()

FetchContent_Declare(
    cglm
    GIT_REPOSITORY "https://github.com/recp/cglm.git"
    GIT_TAG "v0.8.4"
)
FetchContent_MakeAvailable(cglm)

find_package(Vulkan REQUIRED)

function(shader_target NAME)
    cmake_parse_arguments(
        PARSE_ARGV 0 "SHADER_TARGET" "" "VERTEX_SHADER;FRAGMENT_SHADER" ""
    )
    string(APPEND SHADER_TARGET_VERTEX_SPIRV "${SHADER_TARGET_VERTEX_SHADER}"
           ".spv"
    )
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" SHADER_TARGET_VERTEX_SPIRV
                   "${SHADER_TARGET_VERTEX_SPIRV}"
    )
    cmake_path(
        REMOVE_FILENAME SHADER_TARGET_VERTEX_SPIRV OUTPUT_VARIABLE
        SHADER_TARGET_VERTEX_BASE_FOLDER
    )
    string(APPEND SHADER_TARGET_FRAGMENT_SPIRV
           "${SHADER_TARGET_FRAGMENT_SHADER}" ".spv"
    )
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" SHADER_TARGET_FRAGMENT_SPIRV
                   "${SHADER_TARGET_FRAGMENT_SPIRV}"
    )
    cmake_path(
        REMOVE_FILENAME SHADER_TARGET_FRAGMENT_SPIRV OUTPUT_VARIABLE
        SHADER_TARGET_FRAGMENT_BASE_FOLDER
    )
    add_custom_target(
        "${NAME}"
        DEPENDS "${SHADER_TARGET_VERTEX_SHADER}"
                "${SHADER_TARGET_FRAGMENT_SHADER}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory
                "${SHADER_TARGET_VERTEX_BASE_FOLDER}"
        COMMAND "${Vulkan_GLSLC_EXECUTABLE}" -o "${SHADER_TARGET_VERTEX_SPIRV}"
                "${SHADER_TARGET_VERTEX_SHADER}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory
                "${SHADER_TARGET_FRAGMENT_BASE_FOLDER}"
        COMMAND
            "${Vulkan_GLSLC_EXECUTABLE}" -o "${SHADER_TARGET_FRAGMENT_SPIRV}"
            "${SHADER_TARGET_FRAGMENT_SHADER}"
    )
endfunction()

shader_target(
    simple_shader
    VERTEX_SHADER "${PROJECT_SOURCE_DIR}/resources/shaders/simple_shader.vert"
    FRAGMENT_SHADER "${PROJECT_SOURCE_DIR}/resources/shaders/simple_shader.frag"
)

add_executable(first_app source/main.c source/first_app.c source/window.c)
target_link_libraries(first_app PRIVATE glfw Vulkan::Vulkan cglm)
target_compile_definitions(first_app PRIVATE GLFW_INCLUDE_VULKAN)
target_include_directories(first_app PRIVATE include)
add_dependencies(first_app simple_shader)
