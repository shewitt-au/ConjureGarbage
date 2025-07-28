cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)

project(main CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
    sort_checks.cpp
    main.cpp
)
