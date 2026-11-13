cmake_minimum_required(VERSION 3.10)
project(Growhaven)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(game 
    src/core/main.cpp
    src/core/Game.cpp
    src/core/KeyHandler.cpp
    src/core/CollisionChecker.cpp
    src/map/MapManager.cpp
    src/entity/Entity.cpp
    src/entity/Player.cpp
)

target_link_libraries(game 
    SFML::Graphics 
    SFML::Window 
    SFML::System
    SFML::Audio
)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(game PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/SFML/bin"
    $<TARGET_FILE_DIR:game>
)