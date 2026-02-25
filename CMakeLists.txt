# Matrix Calculator - C++ linear algebra app with Qt 6 GUI
# Requires C++17, Qt 6 Widgets. Builds single executable "MatrixApp".

cmake_minimum_required(VERSION 3.16)
project(MatrixCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 6
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(MatrixApp
  src/main.cpp
  src/MainWindow.cpp
  src/matrix.cpp
  src/fraction.cpp
)

target_include_directories(MatrixApp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MatrixApp PRIVATE
  Qt6::Widgets
)

# Install (optional)
install(TARGETS MatrixApp RUNTIME DESTINATION bin)
