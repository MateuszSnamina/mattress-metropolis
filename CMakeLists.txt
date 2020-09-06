cmake_minimum_required(VERSION 3.12)

#################################################################
include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

#################################################################
if (NOT (UNIX OR CYGWIN))
  message(WARNING "The project is supposed to be compiled on linux.")
endif()
if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  message(WARNING "The project is supposed to be compiled with gcc.")
endif()

#################################################################
# On Ubuntu 18.04:
# version=3.14
# build=5
# mkdir ~/temp
# cd ~/temp
# wget https://cmake.org/files/v$version/cmake-$version.$build-Linux-x86_64.sh 
# sudo mkdir /opt/cmake
# sudo sh cmake-$version.$build-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
# sudo ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

#################################################################
project(matterss-metropolis LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall)
add_compile_options(-Wno-unused-function)
add_compile_options(-pedantic)
#add_compile_options(-ffast-math)
add_compile_options(-pthread)

#################################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#################################################################
enable_testing()

#################################################################
message("-----------------------------------")
message("---  boost - imported data  -------")
message("-----------------------------------")

#set(BOOST_ROOT "/usr/include/") # a hint for FindBoost.cmake
#set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/") # a hint for FindBoost.cmake
#set(Boost_NO_SYSTEM_PATHS ON)
#set(Boost_USE_STATIC_LIBS ON)

#find_package(Boost REQUIRED system program_options)
find_package(Boost REQUIRED program_options)
message("\${Boost_FOUND}: ${Boost_FOUND}")

message("\${Boost_VERSION}: ${Boost_VERSION}")
message("\${Boost_LIBRARY_DIRS}: ${Boost_LIBRARY_DIRS}")
message("\${Boost_INCLUDE_DIRS}: ${Boost_INCLUDE_DIRS}")
message("\${Boost_LIBRARIES}:    ${Boost_LIBRARIES}")
if (TARGET Boost::boost)
  message("Boost::boost is a target.")
  get_target_property(x Boost::boost NAME)
    message("  Boost::boost.NAME:                          ${x}")
  get_target_property(x Boost::boost TYPE)
    message("  Boost::boost.TYPE:                          ${x}")
  get_target_property(x Boost::boost INTERFACE_INCLUDE_DIRECTORIES)
    message("  Boost::boost.INTERFACE_INCLUDE_DIRECTORIES: ${x}")
else()
  message(FATAL_ERROR "Boost::boost is not a target!")
endif()

if (TARGET Boost::program_options)
  message("Boost::program_options is a target.")
  get_target_property(x Boost::program_options NAME)
    message("  Boost::program_options.NAME:                          ${x}")
  get_target_property(x Boost::program_options TYPE)
    message("  Boost::program_options.TYPE:                          ${x}")
  get_target_property(x Boost::program_options LOCATION)
    message("  Boost::program_options.LOCATION:                      ${x}")
  get_target_property(x Boost::program_options INTERFACE_INCLUDE_DIRECTORIES)
    message("  Boost::program_options.INTERFACE_INCLUDE_DIRECTORIES: ${x}")
  get_target_property(x Boost::program_options INTERFACE_LINK_LIBRARIES)
    message("  Boost::program_options.INTERFACE_LINK_LIBRARIES:      ${x}")
else()
  message(FATAL_ERROR "Boost::program_options is not a target!")
endif()

#################################################################
message("-----------------------------------")
message("---  armadillo - imported data  ---")
message("-----------------------------------")

#find_package(Armadillo REQUIRED CONFIG PATHS "${CMAKE_SOURCE_DIR}/external_packages/share/Armadillo/CMake" NO_DEFAULT_PATH)
find_package(Armadillo REQUIRED)
message("\${Armadillo_FOUND}: ${Armadillo_FOUND}")

message("\${ARMADILLO_INCLUDE_DIRS}:  ${ARMADILLO_INCLUDE_DIRS}")
message("\${ARMADILLO_LIBRARY_DIRS}:  ${ARMADILLO_LIBRARY_DIRS}")
message("\${ARMADILLO_LIBRARIES}:     ${ARMADILLO_LIBRARIES}")
message("\${ARMADILLO_VERSION_STRING}:${ARMADILLO_VERSION_STRING}")

# if (TARGET ${ARMADILLO_LIBRARIES})
#   message("\${ARMADILLO_LIBRARIES} is a target.")
#   get_target_property(x ${ARMADILLO_LIBRARIES} NAME)
#     message("  \${ARMADILLO_LIBRARIES}.NAME:                          ${x}")
#   get_target_property(x ${ARMADILLO_LIBRARIES} TYPE)
#     message("  \${ARMADILLO_LIBRARIES}.TYPE:                          ${x}")
#   get_target_property(x ${ARMADILLO_LIBRARIES} LOCATION)
#     message("  \${ARMADILLO_LIBRARIES}.LOCATION:                      ${x}")
#   get_target_property(x ${ARMADILLO_LIBRARIES} INTERFACE_INCLUDE_DIRECTORIES)
#     message("  \${ARMADILLO_LIBRARIES}.INTERFACE_INCLUDE_DIRECTORIES: ${x}")
#   get_target_property(x ${ARMADILLO_LIBRARIES} INTERFACE_LINK_LIBRARIES)
#     message("  \${ARMADILLO_LIBRARIES}.INTERFACE_LINK_LIBRARIES:      ${x}")
# else()
#   message(FATAL_ERROR "${ARMADILLO_LIBRARIES} is not a target!")
# endif()

#################################################################
message("-----------------------------------")
message("---  gtest - imported data  -------")
message("-----------------------------------")

# https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/

# On Ubuntu 18.04:
# sudo apt-get install cmake libgtest-dev
# cd /usr/src/gtest
# sudo cmake CMakeLists.txt
# sudo make
# sudo cp *.a /usr/lib

find_package(GTest REQUIRED)
message("\${GTEST_INCLUDE_DIRS}: ${GTEST_INCLUDE_DIRS}")
message("\${GTEST_LIBRARIES}:    ${GTEST_LIBRARIES}")

#################################################################
message("-----------------------------------")
message("---  the project  -----------------")
message("-----------------------------------")

add_subdirectory(extensions)
add_subdirectory(numboard)
add_subdirectory(numboard_tests)
add_subdirectory(metropolis)
