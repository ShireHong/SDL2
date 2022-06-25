# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.15)
cmake_policy(VERSION 3.15)

project ( collision )

#CMAKE_SOURCE_DIR 当前的绝对路径（txt文件的所在路径）
MESSAGE(STATUS "This is CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})

#CMAKE_BINARY_DIR build的绝对路径（生成可执行文件的路径）
MESSAGE(STATUS "This is CMAKE_BINARY_DIR " ${CMAKE_BINARY_DIR})

#CMAKE_CURRENT_BINARY_DIR build的绝对路径
MESSAGE(STATUS "This is CMAKE_CURRENT_BINARY_DIR " ${CMAKE_CURRENT_BINARY_DIR})

#找到sdl2的依赖库
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${collision_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

#这两个命令把*.dll文件放到可执行文件的目录，MAIN_SRC像是一个临时变量
file(GLOB_RECURSE MAIN_SRC ${CMAKE_SOURCE_DIR}/DLL/*.dll)
file(COPY  ${MAIN_SRC} DESTINATION ${CMAKE_BINARY_DIR})


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(include)

#将需要调用到的文件打包到与执行文件一个目录，先把文件重命名
set(RESOURCE ${CMAKE_SOURCE_DIR}/resource)

# 把需要用到的资源拷贝到执行目录下，后面执行文件会用到
file(COPY ${RESOURCE} DESTINATION ${CMAKE_BINARY_DIR})

add_executable( collision src/main.cpp)
target_link_libraries( collision ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})
install(TARGETS collision RUNTIME DESTINATION ${BIN_DIR})