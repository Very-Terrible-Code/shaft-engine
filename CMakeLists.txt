cmake_minimum_required(VERSION 3.9)
project(shaft-game)



add_subdirectory(SHAFT/Vendor/glm)
add_subdirectory(SHAFT/Vendor/glad)
find_package(SDL2 REQUIRED)
find_package(Lua REQUIRED)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Werror  -Wextra -Wpedantic -fomit-frame-pointer ")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(SHAFT/Headers/
                    SHAFT/ImGui/Headers/
                    SHAFT/Vendor/glad/include/
                    SHAFT/Vendor/glfw/include/
                    SHAFT/Vendor/glm/
                    ${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR})


                   
file(GLOB VENDORS_SOURCES SHAFT/Vendor/glad/src/glad.c + SHAFT/ImGui/Sources/*)
file(GLOB PROJECT_HEADERS SHAFT/Headers/*.h)
file(GLOB PROJECT_SOURCES SHAFT/Sources/*.cpp + SHAFT/Sources/editor/img/*.cpp + SHAFT/Sources/game/*.cpp + SHAFT/Sources/editor/*.cpp + SHAFT/Sources/textures/*.cpp + SHAFT/Sources/input/*.cpp + SHAFT/Sources/render/*.cpp + SHAFT/Sources/util/wm/*.cpp + SHAFT/Sources/logic/*.cpp)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} )

target_link_libraries(${PROJECT_NAME} lua glad glm SDL2 -lm -lc
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})


add_custom_command(
                    TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/SHAFT/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>
                    DEPENDS ${PROJECT_SHADERS})
