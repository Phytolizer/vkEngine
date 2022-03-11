cmake_minimum_required(VERSION 3.16...3.23)

project(
    "vkEngine"
    LANGUAGES C
    VERSION "0.1.0"
    DESCRIPTION "Vulkan Engine"
    HOMEPAGE_URL "https://github.com/Phytolizer/vkEngine"
)

find_package(glfw3)
include(FetchContent)

if(NOT glfw3_FOUND)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.3.6"
    )
    FetchContent_MakeAvailable(glfw)
endif()

find_package(Vulkan REQUIRED)

add_executable(hello source/hello.c)
target_link_libraries(hello PRIVATE glfw Vulkan::Vulkan)
