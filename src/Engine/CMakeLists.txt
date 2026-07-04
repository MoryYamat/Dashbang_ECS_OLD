cmake_minimum_required(VERSION 3.10)

add_library(ddknd STATIC)


file(GLOB_RECURSE ddknd_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

target_sources(ddknd PRIVATE ${ddknd_SRC})

target_include_directories(ddknd 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
 
 
# #vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
 
# link
target_link_libraries(ddknd 
    PUBLIC
        glfw
        glm::glm
        fmt::fmt
        glad::glad
        spdlog::spdlog
)


target_compile_features(ddknd PUBLIC cxx_std_20)

# asset copy
set(assets_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../assets")
set(assets_dst_dir "${CMAKE_BINARY_DIR}/bin/assets")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${assets_src_dir}"
            "${assets_dst_dir}"
    COMMENT "Copy assets -> ${assets_dst_dir}"
)

add_dependencies(ddknd copy_assets)