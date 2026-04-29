cmake_minimum_required(VERSION 3.10.0)
project(bread VERSION 0.1.0 LANGUAGES C CXX)

add_executable(bread src/cli.cpp src/compiler.cpp src/compiler.hpp)
add_executable(bread_developer_suite src/gui/gui.cpp src/compiler.cpp src/libs/imgui/imgui_draw.cpp src/libs/imgui/imgui_impl_glfw.cpp src/libs/imgui/imgui_impl_opengl3.cpp src/libs/imgui/imgui_tables.cpp src/libs/imgui/imgui_widgets.cpp src/libs/imgui/imgui.cpp src/libs/imguifd/ImGuiFileDialog.cpp src/libs/imgui/imgui_stdlib.cpp)
# todoadd_executable(bread_tests src/tests.cpp src/compiler.cpp)
target_include_directories(bread PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/libs)
target_include_directories(bread_developer_suite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/libs ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/imgui)
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(bread_developer_suite 
    PRIVATE 
    glfw
    GL
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
