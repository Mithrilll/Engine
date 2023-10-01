cmake_minimum_required(VERSION 3.0)

project(Engine)

add_executable(Engine
    Input.cpp 
    MathLib.cpp 
    Objects.cpp
    Scene.cpp
    main.cpp
)

target_link_libraries(Engine 
    PRIVATE 
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-system)