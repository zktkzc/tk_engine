file(GLOB_RECURSE PLATFORM_SRC src/*.cpp)
add_library(tk_platform STATIC ${PLATFORM_SRC})
target_include_directories(tk_platform PUBLIC include External)

add_subdirectory(External/glfw)
target_link_libraries(tk_platform PRIVATE glfw)