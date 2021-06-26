#include "BoardState.h"

namespace Turbofan
{
	
	BoardState::BoardState()
	{
		pieces.wKing = 0;
		pieces.bKing = 0;

		pieces.wQueen = 0;
		pieces.wRook = 0;
		pieces.wBishop = 0;
		pieces.wKnight = 0;
		pieces.wPawn = 0;

		pieces.bQueen = 0;
		pieces.bRook = 0;
		pieces.bBishop = 0;
		pieces.bKnight = 0;
		pieces.bPawn = 0;

		plyNumber = 0;
	}

	BoardState::~BoardState()
	{

	}

	//no promotion support yet
	void BoardState::makeMove(Ply ply)
	{
		//assumption: move is legal
		uint64_t fromChange = (((uint64_t)1) << ply.from);
		uint64_t toChange = (((uint64_t)1) << ply.to);

		for (unsigned int i = 0; i < 12; i++)
		{
			if (piecesArray[i] & toChange)
			{
				piecesArray[i] ^= toChange;
				break;
			}
		}

		for (unsigned int i = 0; i < 12; i++)
		{
			if (piecesArray[i] & fromChange)
			{
				piecesArray[i] ^= fromChange;
				piecesArray[i] |= toChange;
				break;
			}
		}
	}
	// actually tricky
	//no promotion support yet either
	void BoardState::undoMove(Ply ply)
	{
		//assumption: ply is the last ply played
		uint64_t fromChange = (((uint64_t)1) << ply.from);
		uint64_t toChange = (((uint64_t)1) << ply.to);
		for (unsigned int i = 0; i < 12; i++)
		{
			if (piecesArray[i] & toChange)
			{
				piecesArray[i] ^= toChange;
				piecesArray[i] |= fromChange; //technically could also be xor
				break;
			}
		}

		if (ply.capture)
		{
			piecesArray[ply.capturedPiece] |= toChange;
		}
	}

	void BoardState::setFromFEN(char* FEN)
	{
		//assumption: FEN string is legit
		// currently quick and dirty mockup without proper castling support
		// TODO: fix castling
		uint8_t curIndex = 56;

		uint64_t e = ((uint64_t)1);
		while (*FEN != ' ')
		{
			e = ((uint64_t)1) << curIndex;
			switch (*FEN)
			{
				// black pieces
			case 'k':
				pieces.bKing |= e;
				break;
			case 'q':
				pieces.bQueen |= e;
				break;
			case 'r':
				pieces.bRook |= e;
				break;
			case 'b':
				pieces.bBishop |= e;
				break;
			case 'n':
				pieces.bKnight |= e;
				break;
			case 'p':
				pieces.bPawn |= e;
				break;

				// white pieces
			case 'K':
				pieces.wKing |= e;
				break;
			case 'Q':
				pieces.wQueen |= e;
				break;
			case 'R':
				pieces.wRook |= e;
				break;
			case 'B':
				pieces.wBishop |= e;
				break;
			case 'N':
				pieces.wKnight |= e;
				break;
			case 'P':
				pieces.wPawn |= e;
				break;

			case '/':
				curIndex -= 16;
				break;
			}

			if (*FEN != '/')
			{
				/*bQueen <<= 1;
				bRook <<= 1;
				bBishop <<= 1;
				bKnight <<= 1;
				bPawn <<= 1;

				wQueen <<= 1;
				wRook <<= 1;
				wBishop <<= 1;
				wKnight <<= 1;
				wPawn <<= 1;*/

				curIndex++;
			}
			
			if (*FEN >= '1' && *FEN <= '8')
			{
				curIndex += *FEN - '1';
			}

			FEN++;
		}
		// pieces and vacancies done, now other info like castling, en passant, who to move
		plyNumber = *(++FEN) == 'w' ? 0 : 1;
		FEN++;
		// do castling checks etc...
	}

	std::ostream& operator<<(std::ostream& os, const BoardState& board)
	{
		uint64_t bitboard = board.pieces.wKing | board.pieces.bKing | board.pieces.wQueen | board.pieces.wRook | board.pieces.wBishop | board.pieces.wKnight | board.pieces.wPawn | board.pieces.bQueen | board.pieces.bRook | board.pieces.bBishop | board.pieces.bKnight | board.pieces.bPawn;
		
		/*for (unsigned int i = 1; i < 65; i++)
		{
			os << ((bool)(bitboard % 2) ? "1" : "0") << (i % 8 == 0 ? "\n" : " ");
			bitboard >>= 1;
		}*/

		for (int y = 7; y >= 0; y--)
		{
			for (int x = 0; x <= 7; x++)
			{
				bool cur = (bitboard >> (x + y * 8)) & 1;
				os << (cur ? "1" : "0") << (x == 7 ? "\n" : " ");
			}
		}

		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Ply& ply)
	{
		os << "Move:" << std::endl << "From: " << (int)ply.from << std::endl << "To: " << (int)ply.to << std::endl;
		return os;
	}

	void BoardState::generateLegalMoves()
	{
		
	}
}