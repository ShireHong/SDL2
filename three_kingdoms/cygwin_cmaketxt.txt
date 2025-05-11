cmake_minimum_required(VERSION 3.14)
project(pcieframe)

# 1. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 查找 SDL2（可选）
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# 3. 定义源码目录
set(SOURCE_ROOT src)

# 4. 自动递归查找所有 .cpp 和 .h 文件
file(GLOB_RECURSE SOURCES 
    "${SOURCE_ROOT}/*.cpp" 
    "${SOURCE_ROOT}/*.h"
)

# 5. 添加可执行文件
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
)


# 6. 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SOURCE_ROOT}
    ${SDL2_INCLUDE_DIRS}  # 如果用了 SDL2
)

# 启用 pkg-config 支持
find_package(PkgConfig REQUIRED)

# 通过 pkg-config 查找 SDL2 和 SDL2_image
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)  # 注意大小写

# 7. 链接库（如 SDL2）
#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
#find_package(SDL2_ttf REQUIRED)
#find_package(SDL2_mixer REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE 
							${SDL2_INCLUDE_DIRS}
							${SDL2_IMAGE_INCLUDE_DIRS}
                             /usr/include/SDL2 )
target_link_libraries(${PROJECT_NAME} 
					  ${SDL2_LIBRARIES}
					  ${SDL2_IMAGE_LIBRARYS} 
					  ${SDL2_TTF_LIBRARYS} 
					  ${SDL2_MIXER_LIBRARYS}
                      SDL2_image  # 必须显式添加
                      )

# 8. （可选）复制资源文件（如 assets/）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

# 针对Cygwin的特殊设置
if(CYGWIN)
    # 添加Cygwin特定的链接选项
    target_link_options(${PROJECT_NAME} PRIVATE
         -Wl,-subsystem,console version.o
    )
    
    # 确保链接器能找到库路径
    link_directories(/usr/lib/w32api)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
