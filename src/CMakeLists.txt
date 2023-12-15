cmake_minimum_required(VERSION 3.16)
project(arkanoid VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} main.cpp)


SET( clearEventsQueue.hpp
   )
   SET( clearEventsQueue.cpp
   )

   target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
   target_link_libraries(CMakeSFMLProject PRIVATE sfml-audio)
   target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)
   
   
   install(TARGETS CMakeSFMLProject)