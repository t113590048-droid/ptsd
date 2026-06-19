cmake_minimum_required(VERSION 3.16)
project(REPLACE_WITH_YOUR_PROJECT_NAME)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5")
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        ptsd
        URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip
        SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

# 下載並設定 Box2D
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(box2d)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

add_executable(${PROJECT_NAME}
        ${SRC_DIR}/main.cpp
        ${SRC_FILES}
        ${HEADER_FILES}
        include/StartMenu.hpp
        src/StartMenu.cpp
        include/Gameplay.hpp
        src/Gameplay.cpp
        include/Fruit.hpp
        src/Fruit.cpp
        include/FruitFactory.hpp
        src/FruitFactory.cpp
        src/ContactListener.cpp
        include/ContactListener.hpp
        include/GameOverManager.hpp
        src/GameOverManager.cpp
        include/UIManager.hpp
        src/UIManager.cpp
        include/Cloud.hpp
        src/Cloud.cpp
        include/PauseManager.hpp
        src/PauseManager.cpp
        include/Obstacle.hpp
        include/StaticObstacle.hpp
        include/LevelType.hpp
        include/LevelSelectMenu.hpp
        src/LevelSelectMenu.cpp
        include/MovingObstacle.hpp
        include/PortalObstacle.hpp
        include/CheatManager.hpp
        src/CheatManager.cpp
        include/OpeningAnimation.hpp
        src/OpeningAnimation.cpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
        SDL2::SDL2main
        PTSD
        box2d
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        "${CMAKE_CURRENT_BINARY_DIR}/Resources"
        COMMENT "正在將 Resources 資料夾複製到執行檔目錄以防止破圖..."
)