# Chess-Engine
Chess Engine in C++17.
# Dependencies
- [cmake](https://cmake.org)
# Building
The engine works with GCC, Clang or ICC (tested only with GCC though).
<br>To build, run this in a terminal (from the root directory of the project):
<br>`mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .`
<br> Depending on the platform, you might want to specify the compiler with the option `-DCMAKE_CXX_COMPILER=g++`.
<br><br> This produces three binary files: `chess_perft`, `chess_gui` and `chess_tests`.
<br> `chess_perft` and `chess_tests` are used only for debugging purposes, use `chess_gui` to play chess with a graphical display.
