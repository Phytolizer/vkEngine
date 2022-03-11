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

add_executable(hello source/hello.c)
target_link_libraries(hello PRIVATE glfw Vulkan::Vulkan cglm)
target_compile_definitions(hello PRIVATE GLFW_INCLUDE_VULKAN)
