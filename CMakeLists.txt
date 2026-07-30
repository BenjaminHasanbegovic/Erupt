cmake_minimum_required(VERSION 3.20)
project(Erupt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_DEBUG "Enable debug features." ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(vendor/VMA)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "main.cpp"
        "src/*.cpp"
)

add_executable(Erupt ${SOURCES}
        src/vulkanLogic/VMA/vulkanAllocator.cpp)

target_include_directories(Erupt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Erupt PRIVATE
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
        glfw
        Threads::Threads
        m
        dl
)

if(ENABLE_DEBUG)
    target_compile_definitions(Erupt PRIVATE ERUPT_DEBUG=1)
else()
    target_compile_definitions(Erupt PRIVATE ERUPT_DEBUG=0)
endif()