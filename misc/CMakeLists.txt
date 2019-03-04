cmake_minimum_required(VERSION 2.8)

project(test_tmp)


#add_executable(a a.cc)
#target_compile_options(a PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11 -g>)
#target_compile_options(a PUBLIC -std=c++11 -g)
#set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")


aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
message("source lists:${SRC_LIST}")

link_libraries(-pthread)

foreach(var ${SRC_LIST})
    string(REGEX REPLACE ".*/" "" var ${var})
    message("src:${var}")
    string(REGEX REPLACE ".cpp$|.cc$|.c$" "" target ${var})
    message("target:${target}")
    
    add_executable(${target} ${var})
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-std=c++11>")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-g>")

endforeach(var)
