cmake_minimum_required (VERSION 3.10)
project(TestTask VERSION 1.0.0 LANGUAGES CXX)

add_executable(myApp main.cpp)
set_target_properties(
    myApp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(myApp libpqxx.a -lpq)