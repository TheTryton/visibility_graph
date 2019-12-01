cmake_minimum_required(VERSION 3.8)
project(visualization)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE visualization_sources 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${visualization_sources})

add_executable(visualization ${visualization_sources})
target_link_libraries(visualization Qt5::Core Qt5::Gui Qt5::Widgets algorithm)
target_include_directories(visualization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(visualization PROPERTIES CXX_STANDARD 20)
set_target_properties(visualization PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(visualization PROPERTIES FOLDER visibility_graph)