cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(flappybird VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME}
    src/main.cpp 
    src/Application.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ext/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ext/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw3dll
    vulkan-1
)


set(DLL-FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/lib/glfw3.dll
)

foreach(file IN LISTS DLL-FILES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${file} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "copying dll file"
    )
endforeach()


