file(GLOB_RECURSE PLATFORM_SRC src/*.cpp)
add_library(tk_platform STATIC ${PLATFORM_SRC})
target_include_directories(tk_platform PUBLIC include External)

add_subdirectory(External/glfw)
target_link_libraries(tk_platform PRIVATE glfw)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message("--->Find Vulkan success : ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(tk_platform PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(tk_platform PRIVATE ${Vulkan_LIBRARIES})
endif ()