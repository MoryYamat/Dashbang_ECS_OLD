cmake_minimum_required(VERSION 3.10)

# プロジェクト名とC++標準の設定
project(DashBang_ECS)

option(ENGINE_ENABLE_PROFILING "Enable in-engine profiling" OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 外部ライブラリ設定
# ============================================================

# GLFW（インポート：事前ビルド済み静的ライブラリ）
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/GLFW/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/GLFW/include"
)

# ASSIMP
add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/Assimp/lib/assimp-vc143-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Assimp/include"
)
# set(ASSIMP_DLL "${CMAKE_SOURCE_DIR}/External/Assimp/bin/Release/assimp-vc143-mt.dll")

# ASSIMP
add_library(assimp_local STATIC IMPORTED)
set_target_properties(assimp_local PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/Assimp/lib/assimp-vc143-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Assimp/include"
)

# zlib
add_library(zlibstatic_local STATIC IMPORTED)
set_target_properties(zlibstatic_local PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/Assimp/lib/zlibstatic.lib"
)

# miniaudio
add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE 
    ${CMAKE_SOURCE_DIR}/External/miniaudio
)

# GLAD（ソースからビルド）
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/External/GLAD/src/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/External/GLAD/include
)

# cgltf
add_library(cgltf STATIC
    ${CMAKE_SOURCE_DIR}/src/thirdparty/cgltf/cgltf_impl.c
)
# ヘッダを公開
target_include_directories(cgltf PUBLIC
    ${CMAKE_SOURCE_DIR}/External/cgltf
)
# C としてコンパイル
set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/src/thirdparty/cgltf/cgltf_impl.c
    PROPERTIES LANGUAGE C
)

# stb_image
add_library(stb_image STATIC
    ${CMAKE_SOURCE_DIR}/src/thirdparty/stb/stb_impl.cpp
)
target_include_directories(stb_image PUBLIC
    ${CMAKE_SOURCE_DIR}/External/stb
)

#
if(MSVC)
    target_compile_definitions(stb_image PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(cgltf PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# ============================================================
# プロジェクトのソース・ヘッダー
# ============================================================

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SRC_FILES 
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.hpp"

        )

# 実行可能ファイルの作成
add_executable(DashBang_ECS ${SRC_FILES} ${HEADER_FILES})
# add_executable(ClashBattle
#     src/main.cpp
#     src/Shader.cpp
#     src/Camera.cpp
#     include/Shader.h
#     include/Camera.h
#     include/Component.h
#     include/ECS.h
# )




# インクルードディレクトリ
target_include_directories(DashBang_ECS PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/External/GLFW/include
    ${CMAKE_SOURCE_DIR}/External/GLAD/include
    ${CMAKE_SOURCE_DIR}/External/GLM
    ${CMAKE_SOURCE_DIR}/External/stb
    ${CMAKE_SOURCE_DIR}/External/Assimp/include  
)

file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_BINARY_DIR}")




# リンクするライブラリ
target_link_libraries(DashBang_ECS
    glfw
    glad
    assimp_local
    zlibstatic_local
    opengl32
    cgltf
    stb_image
    miniaudio
)


if(EXISTS "${ASSIMP_DLL}")
    add_custom_command(TARGET DashBang_ECS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSIMP_DLL}"
            "$<TARGET_FILE_DIR:DashBang_ECS>"
    )
else()
    message(WARNING "Assimp DLL not found: ${ASSIMP_DLL}")
endif()

# Debug構成のとき`DEBUG`を定義
target_compile_definitions(DashBang_ECS
    PRIVATE
        GLM_ENABLE_EXPERIMENTAL
        $<$<CONFIG:Debug>:DEBUG>
	$<$<CONFIG:Debug>:ENGINE_ENABLE_PROFILING=1>
	$<$<BOOL:${ENGINE_ENABLE_PROFILING}>:ENGINE_ENABLE_PROFILING=1>
)
