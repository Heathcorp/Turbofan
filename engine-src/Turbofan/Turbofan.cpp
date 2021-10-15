#include <iostream>
#include <string>
#include "MoveTree.h"

int main()
{
	//todo: redo legal move gen

	Turbofan::BoardState testBoard;
	testBoard.setFromFEN((char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//std::cout << sizeof(testBoard) << std::endl;

	Turbofan::MoveTree testTree(testBoard);

	//Turbofan::Ply testPly(1, 18, false, false, 0, 0);
	/*std::cout << sizeof(testPly) << std::endl;

	testBoard.generateLegalMoves();

	Turbofan::MoveTree::Node* testNode = testTree.rootNode->AddChild(testPly);

	std::cout << testBoard << std::endl;
	testBoard.makeMove(testPly);
	std::cout << testPly << std::endl;
	std::cout << testBoard << std::endl;
	testBoard.generateLegalMoves();

	testBoard.undoMove(testPly);
	std::cout << testBoard << std::endl;*/

	std::string input = "illegal";

	std::cout << "Which pieces am I playing? (white or black)" << std::endl;
	std::getline(std::cin, input);

	if (input == "black")
	{
		std::cout << "I am currently unable to play as black, I will play as white instead." << std::endl;
	}

	uint16_t depth = 1;
	
	Turbofan::MoveTree::Node* currentNode = testTree.rootNode;
	currentNode->evaluate(depth, &testBoard, true);

	Turbofan::Ply opponentMove;
	Turbofan::Ply myMove = currentNode->bestPly;
	input = "test";
	std::cout << "Play " << myMove << std::endl;

	while (input != "exit")
	{
		if (input == "illegal")
		{
			testBoard.generateLegalMoves();
			myMove = testBoard.legalMoves.at(rand() % testBoard.legalMoves.size());

			std::cout << "Play " << myMove << std::endl;
		}
		else if (input.size() == 4 && input.at(0) >= 'a' && input.at(0) <= 'h' && input.at(1) >= '1' && input.at(1) <= '8' && input.at(2) >= 'a' && input.at(2) <= 'h' && input.at(3) >= '1' && input.at(3) <= '8')
		{
			testBoard.makeMove(myMove);
			currentNode = currentNode->AddChild(myMove);

			opponentMove.from = input.at(0) - 'a' + (input.at(1) - '1') * 8;
			opponentMove.to = input.at(2) - 'a' + (input.at(3) - '1') * 8;

			testBoard.makeMove(opponentMove);
			currentNode = currentNode->AddChild(opponentMove);

			std::cout << testBoard << std::endl;

			currentNode->evaluate(depth, &testBoard, true);
			myMove = currentNode->bestPly;

			std::cout << "Play " << myMove << std::endl;
			
		}

		std::cout << "Enter opponent's move:" << std::endl;
		std::getline(std::cin, input);
	}
}