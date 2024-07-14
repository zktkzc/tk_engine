add_library(glfw INTERFACE)
target_include_directories(glfw INTERFACE .)
target_link_directories(glfw INTERFACE lib)
target_link_libraries(glfw INTERFACE glfw3dll)

file(COPY_FILE lib/glfw3.dll ${CMAKE_BINARY_DIR}/bin/glfw3.dll)