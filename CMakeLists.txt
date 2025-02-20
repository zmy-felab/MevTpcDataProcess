# CMakeList.txt : CMake project for fec_data_dec, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.10)

project ("MevTpcDataProcessLinux")

set(CMAKE_BUILD_TYPE Debug)

find_package(ROOT REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)  #路径下所有.cc文件放在sources变量中
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/include/*.h)  #路径下所有.h文件放在headers变量中

 # 添加构建目标， 动态链接库 名字叫做hello
 # 最后在Windows下生成 动态链接库 libhello.dll
 # ADD_LIBRARY(MevTpcDataProcessLib SHARED ${sources})
 # 设定动态链接库的存储路径
 # SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/out/build/x64-debug)

# Add source to this project's executable.
add_executable (${CMAKE_PROJECT_NAME} ${sources} ${headers}) #构建可执行文件
target_link_libraries(${CMAKE_PROJECT_NAME} ${ROOT_LIBRARIES}) #链接第三方库


# target_link_libraries(MevTpcDataProcess)
# TODO: Add tests and install targets if needed.

