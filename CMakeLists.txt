cmake_minimum_required(VERSION 3.20)

project(MyProject
    VERSION 1.0
    LANGUAGES CXX
)

# -------------------------------
# C++ standard & build options
# -------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# -------------------------------
# FetchContent: Dependencies
# -------------------------------
include(FetchContent)

# ---- SDL3 ----
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL3)

# ---- SDL3_image ----
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.0
)
# Ensure SDL3_image sees SDL3
set(SDL3_DIR ${sdl3_SOURCE_DIR}/build CACHE PATH "Path to SDL3 build")
FetchContent_MakeAvailable(SDL3_image)

# ---- SDL3_ttf ----
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.0
)
# Ensure SDL3_ttf sees SDL3
set(SDL3_DIR ${sdl3_SOURCE_DIR}/build CACHE PATH "Path to SDL3 build")
FetchContent_MakeAvailable(SDL3_ttf)

# ---- ImGui ----
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.4
)

FetchContent_MakeAvailable(ImGui)

# ---- ImGui-SDL3 backend ----
# ImGui doesn't use CMake by default, this creates a static library target for it
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp

    # SDL3 + SDL_Renderer backend
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# Link against SDL3
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# -------------------------------
# Optional: Library target
# -------------------------------
add_subdirectory(src)

# -------------------------------
# Executables / Apps
# -------------------------------
add_subdirectory(apps)

# -------------------------------
# Tests
# -------------------------------
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
