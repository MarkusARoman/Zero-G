add_library(MyProject STATIC
    example.cpp
)

target_include_directories(MyProject
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)