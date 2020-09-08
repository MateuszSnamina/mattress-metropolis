message("...................................")
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
message("CMake is about to process: ${TARGET_NAME}")

file(GLOB src "src/*.cpp")
message("  Auto detected src files for target ${TARGET_NAME}: ")
message_fancy_list("${src}")

file(GLOB inc "include/${TARGET_NAME}/*.hpp")
message("  Auto detected hpp files for target ${TARGET_NAME}: ")
message_fancy_list("${inc}")

add_executable("${TARGET_NAME}" ${src} ${inc})
target_include_directories("${TARGET_NAME}" PUBLIC include)
target_link_libraries("${TARGET_NAME}"
  utility
  metropolis_engine
  Boost::boost
  Boost::program_options
)
set_target_properties("${TARGET_NAME}" PROPERTIES OUTPUT_NAME "${TARGET_NAME}${N}x${M}")
set(N 10 CACHE STRING "The board size N.")
set(M 10 CACHE STRING "The board size M.")
configure_file("include/metropolis/nm.hpp.in" "${CMAKE_CURRENT_LIST_DIR}/include/metropolis/nm.hpp" @ONLY)

message("CMake has processed: ${TARGET_NAME}")
