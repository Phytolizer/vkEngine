file(
    RELATIVE_PATH relative_path
    "/${vkEngine_INSTALL_CMAKEDIR}"
    "/${CMAKE_INSTALL_BINDIR}/${vkEngine_NAME}"
)

get_filename_component(prefix "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
set(config_dir "${prefix}/${vkEngine_INSTALL_CMAKEDIR}")
set(config_file "${config_dir}/vkEngineConfig.cmake")

message(STATUS "Installing: ${config_file}")
file(WRITE "${config_file}" "\
get_filename_component(
    _vkEngine_executable
    \"\${CMAKE_CURRENT_LIST_DIR}/${relative_path}\"
    ABSOLUTE
)
set(
    VKENGINE_EXECUTABLE \"\${_vkEngine_executable}\"
    CACHE FILEPATH \"Path to the vkEngine executable\"
)
")
list(APPEND CMAKE_INSTALL_MANIFEST_FILES "${config_file}")
