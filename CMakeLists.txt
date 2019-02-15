cmake_minimum_required(VERSION 3.2)

project(2048)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2)
find_package(SDL2_TTF)

set(SRC_FILES main.cpp game.cpp)

if(SDL2_FOUND AND SDL2_TTF_FOUND)

    message(${SDL2_INCLUDE_DIR})
    message(${SDL2_LIBRARY})
    message(${SDL2_MAIN_LIBRARY})

    message(${SDL2_TTF_INCLUDE_DIR})
    message(${SDL2_TTF_LIBRARY})

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    add_executable(${PROJECT_NAME} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY} ${SDL2_TTF_LIBRARY})


endif()
