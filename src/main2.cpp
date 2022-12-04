#include <Functions.hpp>
#include <array>
#include <vector>
#include <string>
#include <raylib-cpp.hpp>
#include <iostream>

#include "./board.hpp"
#include "./utils.hpp"

using std::array;
using std::vector;
using std::string;

using namespace Chess;

struct ChessPiece {
   Vector2 pos;
   Texture2D *txtr;
};

Vector2 round_vector2(Vector2 v);
int get_piece_index_at(vector<ChessPiece>& pieces, Vector2 p);
Vector2 square_to_coordinates(Square sq);
Square coordinates_to_square(Vector2 v);
void load_chess_textures(array<Texture2D, 12> &txtrs, const array<string, 12> &imgs);
void load_pieces_from_board(Board &b, vector<ChessPiece> &pieces, array<Texture2D, 12> &txtrs);
Chess::PieceType get_promotion_type(raylib::Window &win);

constexpr int pieceSize    = 60;
constexpr int winW         = 8*pieceSize;
constexpr int winH         = 8*pieceSize;
constexpr int piecePadding = pieceSize / 2;
constexpr int pieceMinW    = pieceSize / 2;
constexpr int pieceMinH    = pieceSize / 2;
constexpr int pieceMaxW    = winW - pieceSize / 2;
constexpr int pieceMaxH    = winH - pieceSize / 2;


int main(void) {
   raylib::Window win{winW, winH, "Raylib"};
   SetTargetFPS(60);
   win.SetPosition(40, 100);

   const array<string, 12> chess_imgs {
      "../assets/img/pieces/wn.png",
      "../assets/img/pieces/wb.png",
      "../assets/img/pieces/wr.png",
      "../assets/img/pieces/wq.png",
      "../assets/img/pieces/wk.png",
      "../assets/img/pieces/wp.png",
      "../assets/img/pieces/bn.png",
      "../assets/img/pieces/bb.png",
      "../assets/img/pieces/br.png",
      "../assets/img/pieces/bq.png",
      "../assets/img/pieces/bk.png",
      "../assets/img/pieces/bp.png",
   };
   array<Texture2D, 12> txtrs{};
   load_chess_textures(txtrs, chess_imgs);

   //Board b{ Chess::standard_chess };
   //Board b { "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" };
   //Board b{ "5k2/8/8/3qP3/b7/1N3Q2/8/3K1B2 w - - 0 1" };
   //Board b{ "8/P1p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" };
   //Board b { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 10" };
   Board b{ "8/8/5p2/5k2/8/4KP2/8/8 w - - 99 57" };

   Image board_img = LoadImage("../assets/img/chess-board.png");
   ImageResize(&board_img, 8*pieceSize, 8*pieceSize);
   Texture2D board_txtr = LoadTextureFromImage(board_img);
   UnloadImage(board_img);

   vector<ChessPiece> pieces{};
   pieces.reserve(32);

   load_pieces_from_board(b, pieces, txtrs);

   int selected_piece = -1;
   int selected_piece_moves = -1;
   const Color spm_color = ColorAlpha(raylib::Color(142, 193, 112), 0.75f);

   Vector2 mouse_pos = {0};
   Square from = Chess::NoSquare;
   Square to = Chess::NoSquare;
   PieceType pt = Queen; // promotion type

   while (!win.ShouldClose()) {
      mouse_pos = GetMousePosition();
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         selected_piece = get_piece_index_at(pieces, GetMousePosition());
         if (selected_piece != -1) {
            from = coordinates_to_square(pieces.at(selected_piece).pos);
            for (size_t i = 0; i < b.movelist.size(); ++i) {
               if (from == b.movelist.at(i).pos) {
                  selected_piece_moves = i;
                  break;
               }
            }
         }
      }
      else if (selected_piece != -1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
         to = coordinates_to_square(GetMousePosition());
         // get promotion type if necessary
         if (b.get_piece_type(from) == Pawn && (to <= H1 || to >= A8) && b.is_valid_move(from, to)) {
            pt = get_promotion_type(win);
         }

         if (b.make_move(from, to, pt) == Chess::NoErr) load_pieces_from_board(b, pieces, txtrs);
         else pieces.at(selected_piece).pos = square_to_coordinates(from);

         selected_piece = -1;
         selected_piece_moves = -1;
      }
      if (IsKeyPressed(KEY_U)) {
         if (b.unmake_move() == Chess::NoErr) {
            load_pieces_from_board(b, pieces, txtrs);
         }
      }
      if (selected_piece != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
         if (mouse_pos.x > pieceMinW && mouse_pos.x < pieceMaxW) {
            pieces.at(selected_piece).pos.x = mouse_pos.x - piecePadding;
         }
         if (mouse_pos.y > pieceMinH && mouse_pos.y < pieceMaxH) {
            pieces.at(selected_piece).pos.y = mouse_pos.y - piecePadding;
         }
         if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            pieces.at(selected_piece).pos = square_to_coordinates(from);
            selected_piece = -1;
            selected_piece_moves = -1;
         }
      }

      BeginDrawing();
      {
         win.ClearBackground(RAYWHITE);
         // Draw board
         DrawTexture(board_txtr, 0, 0, WHITE);
         // Draw available moves if a piece is selected
         if (selected_piece_moves != -1) {
            for (Square sq = A1; sq <= H8; ++sq) {
               if (b.movelist.at(selected_piece_moves).possible_moves & sqbb(sq)) {
                  DrawRectangleV(square_to_coordinates(sq),(Vector2) {pieceSize, pieceSize}, spm_color);
               }
            }
         }
         // Draw pieces
         for (int i = 0; size_t(i) < pieces.size(); ++i) {
            if (i != selected_piece) DrawTextureV(*pieces.at(i).txtr, pieces.at(i).pos, WHITE);
         }
         if (selected_piece != -1) DrawTextureV(*pieces.at(selected_piece).txtr, pieces.at(selected_piece).pos, WHITE);
      }
      EndDrawing();
   }
}

Vector2 round_vector2(Vector2 v) {
   Vector2 res = {v.x - ((int) v.x % pieceSize), v.y - ((int) v.y % pieceSize)};
   if (v.x < pieceMinW) res.x = 0;
   if (v.y < pieceMinH) res.y = 0;
   if (v.x > pieceMaxW) res.x = pieceMaxW - piecePadding;
   if (v.y > pieceMaxH) res.y = pieceMaxH - piecePadding;
   return res;
}

int get_piece_index_at(vector<ChessPiece>& pieces, Vector2 p) {
   for (size_t i = 0; i < pieces.size(); ++i) {
      if ((int) pieces.at(i).pos.x == (int) (p.x / pieceSize)*pieceSize
          && (int) pieces.at(i).pos.y == (int) (p.y / pieceSize)*pieceSize) {
         return i;
      }
   }
   return -1;
}

Square coordinates_to_square(Vector2 v) {
   Square sq = (Square) (v.x / pieceSize);
   sq += 8*(7 - (int) (v.y / pieceSize));
   return sq;
}

Vector2 square_to_coordinates(Square sq) {
   return (Vector2) {
      (float) (sq%8) * pieceSize,
      (float) (7 - (int)(sq/8)) * pieceSize,
   };
}

void load_chess_textures(array<Texture2D, 12>& txtrs, const array<string, 12>& imgs) {
   for (size_t i = 0; i < imgs.size(); ++i) {
      Image img = LoadImage(imgs[i].c_str());
      ImageResize(&img, pieceSize, pieceSize);
      txtrs[i] = LoadTextureFromImage(img);
      UnloadImage(img);
   }
}

void load_pieces_from_board(Board &b, vector<ChessPiece> &pieces, array<Texture2D, 12> &txtrs) {
   pieces.clear();
   for (Square sq = A1; sq <= H8; ++sq) {
      const PieceType t = b.get_piece_type(sq);
      if (t != NoType) {
         ChessPiece p = {
            square_to_coordinates(sq),
            &txtrs[t + (b.get_piece_color(sq) == White ? 0 : 6)],
         };
         pieces.emplace_back(p);
      }
   }
}

Chess::PieceType get_promotion_type(raylib::Window& win) {
   Chess::PieceType pt = Chess::Queen;
   constexpr int boxW = 250;
   constexpr int boxH = 250;

   while (!win.ShouldClose()) {
      if (IsKeyPressed(KEY_Q)) break;
      else if (IsKeyPressed(KEY_B)) { pt = Bishop; break; }
      else if (IsKeyPressed(KEY_R)) { pt = Rook; break; }
      else if (IsKeyPressed(KEY_N)) { pt = Knight; break; }

      BeginDrawing();
      DrawRectangle(winW/2 - boxW/2, winH/2 - boxH/2, boxW, boxH, raylib::Color(0, 0, 0, 120));
      raylib::DrawText("<Q> Queen", boxW + 30, boxH + 20, 16, raylib::Color::White());
      EndDrawing();
   }
   return pt;
}
