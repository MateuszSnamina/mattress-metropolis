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
target_link_gtest_and_tested_target("${TARGET_NAME}")

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")

message("CMake has processed: ${TARGET_NAME}")