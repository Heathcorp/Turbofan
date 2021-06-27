#include <iostream>
#include "MoveTree.h"

int main()
{
	//todo: 

	Turbofan::BoardState testBoard;
	testBoard.setFromFEN((char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << sizeof(testBoard) << std::endl;

	Turbofan::MoveTree testTree(testBoard);

	Turbofan::Ply testPly(1, 18, true, false, 11, 0);
	std::cout << sizeof(testPly) << std::endl;

	Turbofan::MoveTree::Node* testNode = testTree.rootNode->AddChild(testPly);

	std::cout << testBoard << std::endl;
	testBoard.makeMove(testPly);
	std::cout << testBoard << std::endl;
	testBoard.undoMove(testPly);
	std::cout << testBoard << std::endl;
}