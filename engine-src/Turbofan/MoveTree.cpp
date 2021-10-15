#include "MoveTree.h"

namespace Turbofan
{
	MoveTree::Node::Node()
	{
		parent = nullptr;

		lastPly = Ply();
	}
	MoveTree::Node::Node(Node* parentNode, Ply ply)
	{
		parent = parentNode;

		lastPly = ply;
	}
	MoveTree::Node::~Node()
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			delete children.at(i);
		}
	}
	MoveTree::Node* MoveTree::Node::AddChild(Ply ply)
	{
		Node* child = new Node(this, ply);
		children.push_back(child);
		return child;
	}
	MoveTree::MoveTree(BoardState startingBoard)
	{
		initialState = startingBoard;
		rootNode = new Node();
	}
	MoveTree::~MoveTree()
	{
		delete rootNode;
	}

	float MoveTree::Node::evaluate(uint16_t plyDepth, BoardState* board, bool isMyTurn)
	{
		if (board->plyNumber < plyDepth)
		{
			board->generateLegalMoves();
			float bestEval, worstEval;
			Ply bestMove, worstMove;

			bestMove = board->legalMoves.at(0);
			board->makeMove(bestMove);
			bestEval = AddChild(bestMove)->evaluate(plyDepth - 1, board, !isMyTurn);
			board->undoMove(bestMove);
			worstMove = bestMove;
			worstEval = bestEval;

			float curEval;
			Ply curPly;
			for (unsigned int i = 1; i < board->legalMoves.size(); i++)
			{
				curPly = board->legalMoves.at(i);
				board->makeMove(curPly);
				curEval = AddChild(curPly)->evaluate(plyDepth - 1, board, !isMyTurn);
				board->undoMove(curPly);

				worstEval = curEval < worstEval ? curEval : worstEval;
				bestEval = curEval > bestEval ? curEval : bestEval;
			}

			bool isWhite = ((board->plyNumber + 1) % 2);
			if (!isMyTurn)
			{
				bestPly = bestMove;
				return bestEval;
			}
			else
			{
				bestPly = worstMove;
				return worstEval;
			}
		}
		else
		{
			int sum = 0;
			union
			{
				BoardState::Pieces tempPieces;
				uint64_t tempPieceArray[12];
			};
			tempPieces = board->pieces;

			while (tempPieces.wKing)
			{
				if (tempPieces.wKing & 1)
				{
					sum += 20;
				}
				tempPieces.wKing >>= 1;
			}
			while (tempPieces.wQueen)
			{
				if (tempPieces.wQueen & 1)
				{
					sum += 9;
				}
				tempPieces.wQueen >>= 1;
			}
			while (tempPieces.wRook)
			{
				if (tempPieces.wRook & 1)
				{
					sum += 5;
				}
				tempPieces.wRook >>= 1;
			}
			while (tempPieces.wBishop)
			{
				if (tempPieces.wBishop & 1)
				{
					sum += 3;
				}
				tempPieces.wBishop >>= 1;
			}
			while (tempPieces.wKnight)
			{
				if (tempPieces.wKnight & 1)
				{
					sum += 3;
				}
				tempPieces.wKnight >>= 1;
			}
			while (tempPieces.wPawn)
			{
				if (tempPieces.wPawn & 1)
				{
					sum += 1;
				}
				tempPieces.wPawn >>= 1;
			}

			while (tempPieces.bKing)
			{
				if (tempPieces.bKing & 1)
				{
					sum -= 20;
				}
				tempPieces.bKing >>= 1;
			}
			while (tempPieces.bQueen)
			{
				if (tempPieces.bQueen & 1)
				{
					sum -= 9;
				}
				tempPieces.bQueen >>= 1;
			}
			while (tempPieces.bRook)
			{
				if (tempPieces.bRook & 1)
				{
					sum -= 5;
				}
				tempPieces.bRook >>= 1;
			}
			while (tempPieces.bBishop)
			{
				if (tempPieces.bBishop & 1)
				{
					sum -= 3;
				}
				tempPieces.bBishop >>= 1;
			}
			while (tempPieces.bKnight)
			{
				if (tempPieces.bKnight & 1)
				{
					sum -= 3;
				}
				tempPieces.bKnight >>= 1;
			}
			while (tempPieces.bPawn)
			{
				if (tempPieces.bPawn & 1)
				{
					sum -= 1;
				}
				tempPieces.bPawn >>= 1;
			}

			evaluation = (float)sum / 123.0f;
			return evaluation;
		}
	}
}