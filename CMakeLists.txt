cmake_minimum_required(VERSION 3.27)
project(Chess1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp
        Piece.h
        Piece.cpp
        Chessboard_setup.h
        Chessboard_setup.cpp
        update_state.h
        update_state.cpp
)
