package main

import "fmt"
import "strings"
import "github.com/dylhunn/dragontoothmg"

func perft(board *dragontoothmg.Board, depth int) int {
	if (depth == 0) {
		return 1
	}

	moveList := board.GenerateLegalMoves()
   var nodes int = 0

   for _, currMove := range moveList {
      unapplyFunc := board.Apply(currMove)
      i := perft(board, depth - 1)
      nodes += i
		if (depth != 1) {
			fmt.Println(strings.Repeat("-- ", depth), currMove.String(), i)
		}
      unapplyFunc()
   }
	return nodes
}

func main() {
   board := dragontoothmg.ParseFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ")
	fmt.Println("\nTotal: ", perft(&board, 4))
}
