# Chess-Engine
Chess Engine in C++17.
# Dependencies
Cmake will try to download & compile them, but I am not very good with it, so it might fail :)
- [cmake](https://cmake.org)
- [raylib](https://www.raylib.com/)
- [fmt](https://fmt.dev/latest/index.html)
# Building
The engine works with GCC, Clang or ICC (tested only with GCC though).
<br>To build, run this in a terminal (from the root directory of the project):
<br>`mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_WITH_NIX=OFF && cmake --build .`
## Note
If you have [nix](https://github.com/NixOS/nix) installed remove the `-DBUILD_WITH_NIX=OFF` option.

<br><br> This produces three binary files: `chess_perft`, `chess_gui` and `chess_tests`.
<br> `chess_perft` and `chess_tests` are used only for debugging purposes, use `chess_gui` to play chess with a graphical display.
