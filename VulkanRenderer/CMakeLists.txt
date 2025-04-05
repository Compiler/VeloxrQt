cmake_minimum_required(VERSION 3.16)

project(VulkanRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find packages via vcpkg
find_package(Vulkan REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/spirv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_library(VulkanRenderer SHARED
  VulkanRenderer_global.h
  vulkanrenderer.cpp
  vulkanrenderer.h
  test.h
  Vertex.h
  device.h
  device.cpp
  renderer.h
  OrthographicCamera.h
  OrthographicCamera.cpp
  texture.h
  texture.cpp
  TextureTiling.h
  TextureTiling.cpp
  MetalSurfaceHelper.h
  MetalSurfaceHelper.mm
)

target_link_libraries(VulkanRenderer PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Vulkan::Vulkan
    glfw
    OpenImageIO::OpenImageIO
    ${OpenCV_LIBS}
)

target_include_directories(VulkanRenderer PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
find_library(COCOA_LIBRARY Cocoa)
find_library(QUARTZCORE_LIBRARY QuartzCore)

target_link_libraries(VulkanRenderer
    PRIVATE
        ${COCOA_LIBRARY}
        ${QUARTZCORE_LIBRARY}
)

target_compile_definitions(VulkanRenderer PRIVATE VULKANRENDERER_LIBRARY)
target_compile_definitions(VulkanRenderer
    PRIVATE
        PROJECT_ROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
)
#add_custom_command(
#    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/spirv/vert.spv ${CMAKE_CURRENT_SOURCE_DIR}/spirv/frag.spv
#    COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/shaders/passthrough.vert -o ${CMAKE_CURRENT_SOURCE_DIR}/spirv/vert.spv
#    COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/shaders/passthrough.frag -o ${CMAKE_CURRENT_SOURCE_DIR}/spirv/frag.spv
#    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/passthrough.vert ${CMAKE_CURRENT_SOURCE_DIR}/shaders/passthrough.frag
#    COMMENT "Compiling shaders..."
#)

add_custom_target(Shaders ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/spirv/vert.spv ${CMAKE_CURRENT_SOURCE_DIR}/spirv/frag.spv)
add_dependencies(VulkanRenderer Shaders)
