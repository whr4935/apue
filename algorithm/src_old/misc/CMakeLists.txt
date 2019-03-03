# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

include_directories($(PROJECT_SOURCE_DIR)/include)
include_directories($(PROJECT_SOURCE_DIR))


# 生成链接库
add_library (libmisc ${DIR_LIB_SRCS})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(libmisc PROPERTIES OUTPUT_NAME "misc")

target_link_libraries(libmisc libADT)
