#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include "./board.hpp"
#include "./search.hpp"
#include "./str_utils.hpp"

void multi_threaded_search(Chess::Board &b, std::atomic<Chess::MoveEval> &res,
                           std::atomic_bool &stop);

int main() {
  Chess::Board b{};
  std::atomic<Chess::MoveEval> res{};
  std::atomic_bool stop = false;
  std::thread search_thread{};

  std::string command{};
  while (true) {
    std::cin >> command;
    if (command == "quit")
      break;
    else if (command == "uci") {
      std::cout << "uciok\n";
      std::cout << "id name pekka\n";
      std::cout << "id author blackpill0w\n";
    }
    else if (command == "isready")
      std::cout << "readyok\n";
    else if (command == "go") {
      search_thread = std::thread{[&]() { multi_threaded_search(b, res, stop); }};
      search_thread.detach();
    }
    else if (command == "stop") {
      stop = true;
      search_thread.join();
      stop = false;
    }
    else {
      auto tokens = split(command);
      if (tokens[0] == "position")
        b.loadFEN(tokens[1]);
    }
  }
  stop = true;
  if (search_thread.joinable())
    search_thread.join();
}

void multi_threaded_search(Chess::Board &b, std::atomic<Chess::MoveEval> &res,
                           std::atomic_bool &stop) {
  int depth = 0;
  while (! stop) {
    Chess::MoveEval tmp_res = search(b, depth++, stop);
    res                     = tmp_res;
  }
}