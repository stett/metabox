
# Project Name
project(MetaBox)

# Find packaes
find_package(SFML 2 REQUIRED system window graphics) #// network audio)
find_package(Box2D REQUIRED)

# Includes
include_directories(${SFML_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIRS})
include_directories(${MetaBox_SOURCE_DIR})
include_directories(${Frame_SOURCE_DIR}/include)

# Define the executable
file(GLOB Components components/*.cpp)
add_executable(MetaBox main.cpp components.cpp ${Components})

# Make links
target_link_libraries(MetaBox Frame Box2D ${SFML_LIBRARIES})

message("$<TARGET_FILE_DIR:MetaBox>")

add_custom_command(
    TARGET MetaBox
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/textures"
    $<TARGET_FILE_DIR:MetaBox>)
