#include "BoardState.h"

namespace Turbofan
{
	Ply::Ply(uint8_t fromSquare, uint8_t toSquare, bool isCapture, bool isPromotion, uint8_t pieceCaptured, uint8_t promotedPiece)
	{
		from = fromSquare;
		to = toSquare;
		capture = isCapture;
		promotion = isPromotion;
		capturedPiece = pieceCaptured;
		promotionPiece = promotedPiece;
	}

	Ply::Ply()
	{
		from = 0;
		to = 0;
		capture = 0;
		promotion = 0;
		capturedPiece = 0;
		promotionPiece = 0;
	}
	
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

		plyNumber++;
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

		plyNumber--;
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
		os << "Move piece from " << (char)((int)'a' + (int)ply.from % 8) << 1 + (int)ply.from / 8 << " to " << (char)((int)'a' + (int)ply.to % 8) << 1 + (int)ply.to / 8 << std::endl;
		return os;
	}

	void BoardState::generateLegalMoves()
	{
		std::vector<Ply> candidateMoves;
		//player is the side looking to play a move
		//enemy is the side which just made a move
		uint64_t whitePieces = pieces.wKing | pieces.wQueen | pieces.wRook | pieces.wBishop | pieces.wKnight | pieces.wPawn;
		uint64_t blackPieces = pieces.bKing | pieces.bQueen | pieces.bRook | pieces.bBishop | pieces.bKnight | pieces.bPawn;
		uint64_t allPieces = whitePieces | blackPieces;
		
		bool isWhite = ~(plyNumber % 2);

		/*union
		{
			BoardState::Pieces tempPieces;
			uint64_t tempPieceArray[12];
		};
		tempPieces = pieces;*/

		//working masks used multiple times
		uint64_t attackedSquare;
		uint64_t currentSquare = 1;
		if (isWhite)
		{
			for (unsigned int i = 0; i < 64; i++)
			{
				int x = i % 8;
				int y = i / 8;

				if (pieces.wPawn & currentSquare)
				{
					//todo: add en-passant and capture-promotions, maybe make more elegant?

					//pawn forward-moves
					if (!((currentSquare << 8) & allPieces))
					{
						if (i / 8 == 6)
						{
							//promotion
							for (unsigned int j = 1; j < 5; j++)
							{
								candidateMoves.push_back(Ply(i, i + 8, false, true, 0, j));
							}
						}
						else
						{
							//single pawn move
							candidateMoves.push_back(Ply(i, i + 8, false, false, 0, 0));

							//double pawn move
							if ((i / 8 == 1) && !((currentSquare << 16) & blackPieces & whitePieces))
							{
								candidateMoves.push_back(Ply(i, i + 16, false, false, 0, 0));
							}
						}
						
					}
					
					//captures
					attackedSquare = (currentSquare << 7);
					if ((i % 8 != 0) && (attackedSquare & blackPieces))
					{
						for (unsigned int j = 7; j < 12; j++)
						{
							if (piecesArray[j] & attackedSquare)
							{
								candidateMoves.push_back(Ply(i, i + 7, true, false, j, 0));
								break;
							}
						}
					}
					attackedSquare = (currentSquare << 9);
					if ((i % 8 != 7) && (attackedSquare & blackPieces))
					{
						for (unsigned int j = 7; j < 12; j++)
						{
							if (piecesArray[j] & attackedSquare)
							{
								candidateMoves.push_back(Ply(i, i + 9, true, false, j, 0));
								break;
							}
						}
					}
				}
				else if (pieces.wKnight & currentSquare)
				{
					int offsets[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
					for (unsigned int k = 0; k < 8; k++)
					{
						int p = ((int)i + offsets[k]);

						if (p < 64 && p >= 0)
						{
							int px = p % 8;
							int py = p / 8;

							if ((abs(px - x) == 2 && abs(py - y) == 1) || (abs(px - x) == 1 && abs(py - y) == 2))
							{
								attackedSquare = (uint64_t)1 << p;
								if (attackedSquare & blackPieces)
								{
									for (unsigned int j = 7; j < 12; j++)
									{
										if (piecesArray[j] & attackedSquare)
										{
											candidateMoves.push_back(Ply(i, (uint64_t)p, true, false, j, 0));
											break;
										}
									}
								}
								else if (!(attackedSquare & allPieces))
								{
									candidateMoves.push_back(Ply(i, (uint64_t)p, false, false, 0, 0));
								}
							}
						}
					}
				}
				else if (pieces.wBishop & currentSquare)
				{

				}
				else if (pieces.wRook & currentSquare)
				{

				}
				else if (pieces.wQueen & currentSquare)
				{

				}
				else if (pieces.wKing & currentSquare)
				{

				}

				//next square
				currentSquare <<= 1;
			}
		}
		else
		{
			for (unsigned int i = 0; i < 64; i++)
			{
				if (pieces.bPawn & currentSquare)
				{
					
				}
				else if (pieces.bKnight & currentSquare)
				{

				}
				else if (pieces.bBishop & currentSquare)
				{

				}
				else if (pieces.bRook & currentSquare)
				{

				}
				else if (pieces.bQueen & currentSquare)
				{

				}
				else if (pieces.bKing & currentSquare)
				{

				}

				//next square
				currentSquare <<= 1;
			}
		}

		std::cout << "Found " << candidateMoves.size() << " candidate moves" << std::endl;
		for (unsigned int i = 0; i < candidateMoves.size(); i++)
		{
			std::cout << candidateMoves.at(i);
		}
	}
}