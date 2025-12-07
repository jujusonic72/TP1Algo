cmake_minimum_required(VERSION 3.20)
project(TP1Algo)

# Chemins relatifs au projet
set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/SDL3-3.2.26/cmake")
set(SDL3_ttf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/SDL3_ttf-3.2.2/cmake")

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

add_executable(TP1Algo 
    src/TP1Algo.cpp
    src/Command.cpp
    src/QueueHandler.cpp
    src/Player.cpp
    src/Position.cpp
    src/TextBox.cpp
    src/Commands/MoveCommand.cpp
    src/Commands/CancelCommand.cpp
)

# Indiquez où se trouvent vos headers
target_include_directories(TP1Algo PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TP1Algo PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# Copier les assets à côté de l'exécutable
add_custom_command(TARGET TP1Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TP1Algo>/assets
)