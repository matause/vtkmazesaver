cmake_minimum_required(VERSION 2.6)

PROJECT(Final_MazeGame)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(MazeGame Final_MazeGame.cxx ${CMAKE_CURRENT_BINARY_DIR}/Node.h)
TARGET_LINK_LIBRARIES(MazeGame vtkHybrid vtkWidgets)
