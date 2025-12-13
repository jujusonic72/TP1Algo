cmake_minimum_required(VERSION 3.20)
project(TP1Algo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/SDL3-3.2.26/cmake")
set(SDL3_ttf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/SDL3_ttf-3.2.2/cmake")
set(SDL3_image_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/SDL3_image-3.2.4/cmake")

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

add_executable(TP1Algo 
    src/TP1Algo.cpp
    src/Command.cpp
        src/Commands/MoveCommand.cpp
        src/Commands/CancelCommand.cpp
        src/Commands/WaitCommand.cpp
        src/Commands/AttackCommand.cpp
        src/Commands/MoveCommand.cpp
        src/Commands/CancelCommand.cpp
        src/Commands/UseCommand.cpp
        src/Commands/TalkCommand.cpp
    src/QueueHandler.cpp
    src/Player.cpp
    src/Position.cpp
    src/TextBox.cpp
    src/Camera.cpp
    src/Item.cpp
        src/Items/ItemDamageBoost.cpp
        src/Items/ItemHealthPotion.cpp
        src/Items/ItemSpeedBoost.cpp
    src/Inventory.cpp
    src/Enemy.cpp
    src/Pnj.cpp
)

target_include_directories(TP1Algo PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TP1Algo PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

add_custom_command(TARGET TP1Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TP1Algo>/assets
)
add_custom_command(TARGET TP1Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/external/SDL3/SDL3-3.2.26/lib/x64/SDL3.dll $<TARGET_FILE_DIR:TP1Algo>
)
add_custom_command(TARGET TP1Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/external/SDL3/SDL3_ttf-3.2.2/lib/x64/SDL3_ttf.dll $<TARGET_FILE_DIR:TP1Algo>
)
add_custom_command(TARGET TP1Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/external/SDL3/SDL3_image-3.2.4/lib/x64/SDL3_image.dll $<TARGET_FILE_DIR:TP1Algo>
)