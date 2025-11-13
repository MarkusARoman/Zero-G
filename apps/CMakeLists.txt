add_executable(my_app bezier.cpp)

target_link_libraries(my_app PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    imgui
)