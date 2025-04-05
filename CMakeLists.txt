cmake_minimum_required(VERSION 3.16)

project(TestRenderer VERSION 0.1 LANGUAGES CXX)
message(STATUS "OpenCV_DIR is set to: ${OpenCV_DIR}")
if(NOT DEFINED OpenCV_DIR)
  set(OpenCV_DIR "C:/Users/ljuek/Code/vcpkg/installed/x64-windows/share/opencv" CACHE PATH "Path to OpenCV")
  message(STATUS "Manually setting OpenCV_DIR to: ${OpenCV_DIR}")
endif()


set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appTestRenderer
    main.cpp
    vulkanrenderitem.h vulkanrenderitem.cpp
)

qt_add_qml_module(appTestRenderer
    URI TestRenderer
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTestRenderer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTestRenderer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appTestRenderer
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appTestRenderer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(VulkanRenderer)
target_link_libraries(appTestRenderer PRIVATE VulkanRenderer)


