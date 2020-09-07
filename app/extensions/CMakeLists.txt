message("...................................")
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
message("CMake is about to process: ${TARGET_NAME}")

file(GLOB inc "include/${TARGET_NAME}/*.hpp")
message("  Auto detected hpp files for target ${TARGET_NAME}:")
message_fancy_list("${inc}")

add_library("${TARGET_NAME}" INTERFACE)
target_sources("${TARGET_NAME}" INTERFACE ${inc})
target_include_directories("${TARGET_NAME}" INTERFACE include)
target_link_libraries("${TARGET_NAME}" INTERFACE Boost::boost)

message("CMake has processed: ${TARGET_NAME}")
