cmake_minimum_required(VERSION 3.1)
project(Pong C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.c")
file(GLOB_RECURSE HEADERS "${SRC_DIR}/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PRIVATE "src"
    PRIVATE ${SDL2_INCLUDE_DIR}
    PRIVATE ${SDL2TTF_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2TTF_LIBRARY}
    "m"
    ${CMAKE_DL_LIBS}
)
