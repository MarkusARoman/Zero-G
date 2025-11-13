include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # or latest stable version
)
FetchContent_MakeAvailable(Catch2)

add_executable(test_example test_example.cpp)
target_link_libraries(test_example PRIVATE MyProject Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_example)
