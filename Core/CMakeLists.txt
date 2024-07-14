aux_source_directory(src CORE_SRC)
add_library(tk_core STATIC ${CORE_SRC})
target_include_directories(tk_core PUBLIC include)