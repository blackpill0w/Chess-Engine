# Chess-Engine
Chess Engine in C++17.
# Dependencies
- [fmt](https://github.com/fmtlib/fmt)
- [ratlib-cpp](https://github.com/robloach/raylib-cpp)
# TODO
Add dependencies to the build file.
# Building
The engine works (should work) with GCC, Clang or ICC (tested only with GCC though).
<br>Run this in a terminal (from the root directory of the project):
<br>`meson setup --buildtype release build && cd build && meson compile`
<br><br> This produces two binary files, `chess` and `chess-gui`.
<br> `chess` is used only for debugging purposes while `chess-gui` can be used to play chess with a graphical display.
