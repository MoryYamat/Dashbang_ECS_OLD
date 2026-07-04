cmake_minimum_required(VERSION 3.10)

add_library(dashbang STATIC)


file(GLOB_RECURSE dashbang_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

target_sources(dashbang PRIVATE ${dashbang_SRC})

target_include_directories(dashbang 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(dashbang PUBLIC cxx_std_20)

target_link_libraries(dashbang 
    PRIVATE 
        ddknd
)

add_executable(DashBang_ECS
    main.cpp
)

target_compile_features(DashBang_ECS PRIVATE  cxx_std_20)

set_target_properties(DashBang_ECS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)