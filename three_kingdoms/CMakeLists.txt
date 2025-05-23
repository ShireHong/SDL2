# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.15)
cmake_policy(VERSION 3.15)

project (three_kingdoms)

#CMAKE_SOURCE_DIR 当前的绝对路径（txt文件的所在路径）
MESSAGE(STATUS "This is CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})

#CMAKE_BINARY_DIR build的绝对路径（生成可执行文件的路径）
MESSAGE(STATUS "This is CMAKE_BINARY_DIR " ${CMAKE_BINARY_DIR})

#CMAKE_CURRENT_BINARY_DIR build的绝对路径
MESSAGE(STATUS "This is CMAKE_CURRENT_BINARY_DIR " ${CMAKE_CURRENT_BINARY_DIR})

set(DCMAKE_SH="CMAKE_SH-NOTFOUND")

#编译需要用的链接库，静态库
set(EX_LIBS libxml2.a 
	libz.a
	libiconv.a
	libws2_32.a
)

option(WANT_ZLIB "use zlib (ability to decompress layers data) ?" Off)
option(BUILD_SHARED_LIBS "Build shared libraries (dll / so)" Off)


include(CheckIncludeFiles)
CHECK_INCLUDE_FILES("stdint.h" STDINT_H)
if(NOT STDINT_H)
    message(FATAL_ERROR "error: required header stdint.h not found")
endif()


#找到sdl2的依赖库
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${three_kingdoms_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")
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
#有这个就可以把一些h文件和c文件放一起，让后一起编译，不然include里面的h文件编译会报找不到h文件
include_directories( ${CMAKE_SOURCE_DIR}/src/ )

#将需要调用到的文件打包到与执行文件一个目录，先把文件重命名
set(RESOURCE ${CMAKE_SOURCE_DIR}/resource)

# 把需要用到的资源拷贝到执行目录下，后面执行文件会用到
file(COPY ${RESOURCE} DESTINATION ${CMAKE_BINARY_DIR})

#add_subdirectory(src)

#编译可执行文件

aux_source_directory(src/ DIR_SRCS)
aux_source_directory(src/sdl DIR_SRCS_SDL)
aux_source_directory(src/preferences DIR_SRCS_PREF)
aux_source_directory(src/gui DIR_SRCS_GUI)
aux_source_directory(src/gui/widgets DIR_SRCS_GUI_WIDGETS)
aux_source_directory(src/gui/core DIR_SRCS_GUI_CORE)
aux_source_directory(src/gui/dialogs DIR_SRCS_GUI_DIALOGS)
aux_source_directory(src/gui/core/event DIR_SRCS_CORE_EVENT)


set(SOURCE_ALL 
	${DIR_SRCS} 
	${DIR_SRCS_SDL} 
	${DIR_SRCS_PREF}
	${DIR_SRCS_GUI}
    ${DIR_SRCS_GUI_WIDGETS}
    ${DIR_SRCS_GUI_CORE}
    ${DIR_SRCS_GUI_DIALOGS}
	${DIR_SRCS_CORE_EVENT}
	)
#add_executable( three_kingdoms ${DIR_SRCS} ${DIR_SRCS_SDL} ${DIR_SRCS_PREF})
add_executable( three_kingdoms ${SOURCE_ALL} )

#在使用zlib的时候，有一个文件中需要定义WANT_ZLIB这个宏
if(WANT_ZLIB )
    message("WANT_ZLIB define")
    target_compile_definitions( three_kingdoms PRIVATE WANT_ZLIB)    
endif()

target_link_options( three_kingdoms INTERFACE "SHELL:-s USE_ZLIB=1")
target_link_libraries( three_kingdoms ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY} ${EX_LIBS})
install(TARGETS three_kingdoms RUNTIME DESTINATION ${BIN_DIR})
