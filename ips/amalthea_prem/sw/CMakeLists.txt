# cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
# Petalinux uses cmake 3.5.1
cmake_minimum_required(VERSION 3.5)
project(synthetic)

# debug options
option(DEBUG "Enables debug messages" OFF)
option(DEBUG_EXTRA "Enables Even more debug messages" OFF)
option(EMULATE_FPGA_OFFLOADING "Emulates the behavior of a FPGA offloading" OFF)
option(EMULATE_GPU_OFFLOADING "Emulates the behavior of a GPU offloading" OFF)
if (DEBUG)
  add_definitions(-DDEBUG)
endif()
if (DEBUG_EXTRA)
  add_definitions(-DDEBUG_EXTRA)
endif()
if (EMULATE_FPGA_OFFLOADING)
  add_definitions(-DEMULATE_FPGA_OFFLOADING)
endif()
if (EMULATE_GPU_OFFLOADING)
  add_definitions(-DEMULATE_GPU_OFFLOADING)
endif()

# Check the supported CPU archtectures
if (NOT (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR  CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l"))
  message(FATAL_ERROR "Unsupported architecture ${CMAKE_SYSTEM_PROCESSOR}. Only aarch64, armv7l, and x86_64 are supported.")
endif()

# Check the computer arquitecture
# CMAKE_HOST_SYSTEM_VERSION has the value of 'uname -r'
# in NVIDIA Xavier
#   $ uname -r
#   4.9.140-tegra
# in Xilinx Zynq
#   $ uname -r
#   4.9.0-xilinx
# in a generic PC
#   $ uname -r
#   5.4.0-65-generic
STRING(FIND ${CMAKE_HOST_SYSTEM_VERSION} "xilinx" IS_ZYNQ ) 
STRING(FIND ${CMAKE_HOST_SYSTEM_VERSION} "tegra" IS_JETSON ) 
STRING(FIND ${CMAKE_HOST_SYSTEM_VERSION} "generic" IS_PC ) 

if(${IS_JETSON} GREATER 0)
  set(IS_JETSON 1)
  MESSAGE("-- NVIDIA Jetson board detected. Kernel version ${CMAKE_HOST_SYSTEM_VERSION}." )
elseif(${IS_ZYNQ} GREATER 0)
  set(IS_ZYNQ 1)
  MESSAGE("-- Xilinx Zynq board detected. Kernel version ${CMAKE_HOST_SYSTEM_VERSION}." )
elseif(${IS_PC} GREATER 0)
  set(IS_PC 1)
  MESSAGE("-- Generic PC detected. Kernel version ${CMAKE_HOST_SYSTEM_VERSION}." )
else()
  MESSAGE(FATAL_ERROR "Unsupported computer. Expecting NVIDIA Jetson, Xilinx Zynq, or a generic PC.")
endif()

# Check depedencies
find_package(OpenMP 4.5 QUIET)
if(OPENMP_FOUND)
  #message(STATUS "OpenMP version: ${OpenMP_CXX_VERSION}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(CUDA 10.2 QUIET)
if(CUDA_FOUND)
  message(STATUS "CUDA version: ${CUDA_VERSION}")
endif()

# Add the dir to the header files
include_directories(
  /fredsys/fred_lib/fred_support/
  /fredsys/fred_lib/
        # ${PROJECT_SOURCE_DIR}/include/runnables
        # ${PROJECT_SOURCE_DIR}/include/tasks
        # ${PROJECT_SOURCE_DIR}/include/utils
)

# Create a list of sources filenames
file (GLOB SRC_FILES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    # ${PROJECT_SOURCE_DIR}/src/runnables/*.cpp
    # ${PROJECT_SOURCE_DIR}/src/runnables/*.c
    # ${PROJECT_SOURCE_DIR}/src/tasks/*.cpp
    # ${PROJECT_SOURCE_DIR}/src/tasks/*.c
    # ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
    # ${PROJECT_SOURCE_DIR}/src/utils/*.c
)

# Place the executable in the base dir
# The cmake command 'add_executable' must come before the 'target_link_libraries'
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(synthetic ${SRC_FILES})

# check OpenMP offloading support
#if(CUDA_FOUND AND OPENMP_FOUND AND (${IS_JETSON} EQUAL 1 OR ${IS_PC} EQUAL 1))
if(CUDA_FOUND AND OpenMP_C_FOUND AND (IS_JETSON OR IS_PC))
  if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(OMP_OFFLOAD_FLAGS "-fopenmp=libomp -fopenmp-targets=nvptx64-nvidia-cuda")
    # disable some warnings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument -Wno-deprecated-register -Wno-unknown-cuda-version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -Wno-deprecated-register -Wno-unknown-cuda-version")
    message("Using Clang compiler.")
  elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    #set(OMP_OFFLOAD_FLAGS "-fopenmp --target=x86_64-unknown-linux-gnu")
    set(OMP_OFFLOAD_FLAGS "-fopenmp")
    message(WARNING "GPU offloading is not tested with GNU compiler.")
  else()
    message(FATAL_ERROR "Unsupported compiler ${CMAKE_C_COMPILER_ID}. Only GNU and Clang are supported.")
  endif()
  # Define the GPU ID
  add_definitions(-DDEVICE_ID=0)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OMP_OFFLOAD_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OMP_OFFLOAD_FLAGS}")
# TODO: there is no OpenMP driver for Zynq yet. Fix this in the future !
#elseif(OpenMP_C_FOUND AND (${IS_ZYNQ} EQUAL 1))
elseif(IS_ZYNQ)
  # if there is no CUDA in a aarch64 or armv7l core, then it's an FPGA and FRED lib is required
  #add_subdirectory(lib/fred)
  #include_directories(${PROJECT_SOURCE_DIR}/lib/fred)
  include_directories(
    /fredsys/fred_lib/fred_support/
    /fredsys/fred_lib/
  )
  #link_directories("${CMAKE_BINARY_DIR}/lib/fred")
  find_library(FREDLIB
    NAMES fred 
    PATHS /fredsys/fred_lib/)

  target_link_libraries(synthetic ${FREDLIB})
  # gcc 5.3.1 used in Petalinux requires this additional flag to use C++11
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++1y")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")  
else()
  message(FATAL_ERROR "The minimal requirements were not met for compilation.")
endif()

# TODO temporary hack for testing only
# set (IS_ZYNQ 1)
# if (IS_ZYNQ)
#   message(STATUS "-------------- FRED ------------")
#   add_subdirectory(lib/fred)
#   include_directories(${PROJECT_SOURCE_DIR}/lib/fred)
#   link_directories("${CMAKE_BINARY_DIR}/lib/fred")
#   target_link_libraries(synthetic fred)
# endif()

