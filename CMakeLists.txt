cmake_minimum_required(VERSION 3.21)

project(AlchemyWeightReducer VERSION 0.1.0 LANGUAGES CXX)

add_compile_definitions(
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn off CommonLib's unit tests so we don't have to pull catch2
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Pull in CommonLibSSE-NG from our submodule
add_subdirectory(extern/CommonLibSSE-NG CommonLibSSE EXCLUDE_FROM_ALL)
include(extern/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)
find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")

# Build the SKSE plugin DLL
add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        src/pch.h
        src/settings.h
        src/settings.cpp
        src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SIMPLEINI_INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
