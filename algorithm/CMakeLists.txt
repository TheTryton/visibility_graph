cmake_minimum_required(VERSION 3.8)
project(algorithm)

file(GLOB_RECURSE algorithm_sources 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl" 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${algorithm_sources})

add_library(algorithm STATIC ${algorithm_sources})

target_include_directories(algorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(algorithm PROPERTIES CXX_STANDARD 20)
set_target_properties(algorithm PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(algorithm PROPERTIES FOLDER visibility_graph)