#include <iostream>
#include "BoardState.h"

int main()
{
	//todo: fix ply structure maybe make its own class with a proper constructor idk

	Turbofan::BoardState testBoard;
	testBoard.setFromFEN((char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << sizeof(testBoard) << std::endl;

	Turbofan::Ply testPly = { 1, 50, 0x1b, 0 };
	std::cout << sizeof(testPly) << std::endl;

	std::cout << testBoard << std::endl;
	testBoard.makeMove(testPly);
	std::cout << testBoard << std::endl;
	testBoard.undoMove(testPly);
	std::cout << testBoard << std::endl;
}