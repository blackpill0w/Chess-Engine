cmake_minimum_required(VERSION 3.10)
project(Chess-Engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(chess
  src/main.cpp
  src/board.cpp
  src/load_FEN.cpp
  src/move_gen.cpp
  src/pawn_move_gen.cpp
)
