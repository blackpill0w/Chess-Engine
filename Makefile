# To be improved

CXX = g++
CXX_FLAGS = -std=c++17 -Wall -Wextra -Weffc++ -Wfloat-equal
CXX_DEBUG_FLAGS = -ggdb -Og -Werror
CXX_RELEASE_FLAGS = -O2

SRC = src

SOURCE_FILE_LIST = ${SRC}/board.cpp ${SRC}/debug.cpp ${SRC}/main.cpp ${SRC}/move_gen.cpp ${SRC}/pawn_move_gen.cpp

debug:
	${CXX} ${CXX_FLAGS} ${CXX_DEBUG_FLAGS} ${SOURCE_FILE_LIST} -o main_debug
release:
	${CXX} ${CXX_FLAGS} ${CXX_RELEASE_FLAGS} ${SOURCE_FILE_LIST} -o main_release
