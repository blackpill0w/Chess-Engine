#include <iostream>
#include <chrono>

#include "./board.hpp"
#include "./debug.hpp"

using std::cin;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::seconds;
using std::milli;

void timed_perft(Board &b, int depth);

int main(int argc, char **argv) {
   //Board b{ standard_chess };
   //Board b{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" };
   //Board b{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" };
   if (argc == 2) {
      Board b{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" };
      int depth = argv[1][0] - '0';
      timed_perft(b, depth);
      return 0;
   }

   // Perft test
   string fen{};
   cout << "Enter a fen string (std for standard chess): ";
   cin >> fen;
   if (fen == "std") {
      fen = Chess::standard_chess;
   }
   Board b{ fen };

   while (true) {
      int depth = 0;
      cout << "Depth (-1 to exit): ";
      std::cin >> depth;
      if (depth == -1) {
         break;
      }
      timed_perft(b, depth);
   }

   return 0;
}

void timed_perft(Board &b, int depth) {
   auto t1 = high_resolution_clock::now();
   Bitboard nodes = perft(b, depth);
   auto t2 = high_resolution_clock::now();
   duration<double, milli> exec_time_ms = t2 - t1;
   duration<double> exec_time_s = exec_time_ms;
   cout << "Nodes: " << nodes << '\n';
   cout << "Calculated in: " << exec_time_s.count() << "s, or " << exec_time_ms.count() << "ms\n\n" ;
}
