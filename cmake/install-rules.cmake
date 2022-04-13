include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package vkEngine)

install(
    TARGETS vkEngine_exe
    RUNTIME COMPONENT vkEngine_Runtime
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    vkEngine_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(vkEngine_INSTALL_CMAKEDIR)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${vkEngine_INSTALL_CMAKEDIR}"
    COMPONENT vkEngine_Development
)

# Export variables for the install script to use
install(CODE "
set(vkEngine_NAME [[$<TARGET_FILE_NAME:vkEngine_exe>]])
set(vkEngine_INSTALL_CMAKEDIR [[${vkEngine_INSTALL_CMAKEDIR}]])
set(CMAKE_INSTALL_BINDIR [[${CMAKE_INSTALL_BINDIR}]])
" COMPONENT vkEngine_Development)

install(
    SCRIPT cmake/install-script.cmake
    COMPONENT vkEngine_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
