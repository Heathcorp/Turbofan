#include <iostream>
#include <cstdint>
#include <vector>

#pragma once
namespace Turbofan
{

	struct Ply
	{
		uint8_t from : 8;
		uint8_t to : 8;

		uint8_t capturedPiece : 4;
		bool capture : 1;
		uint8_t : 3;
		uint8_t promotionPiece : 4;
		bool promotion : 1;
		uint8_t : 3;

		Ply(uint8_t fromSquare, uint8_t toSquare, bool isCapture, bool isPromotion, uint8_t pieceCaptured, uint8_t promotedPiece);
		Ply();

		friend std::ostream& operator<< (std::ostream& os, const Ply& ply);
	};

	class BoardState
	{
	private:
		/*struct PlayerPieces
		{
			struct King
			{
				uint8 index : 6;
				bool canCastleK : 1;
				bool canCastleQ : 1;
			} king;
			uint64 queen, rook, bishop, knight, pawn;
		};*/
		//PlayerPieces white, black;
	public:
		struct Pieces
		{
			uint64_t wKing, wQueen, wRook, wBishop, wKnight, wPawn;
			uint64_t bKing, bQueen, bRook, bBishop, bKnight, bPawn;
		};
		// kings, queens, rooks, bishops, knights and pawns for each colour;
		union
		{
			Pieces pieces;
			uint64_t piecesArray[12];
		};
		

		uint16_t plyNumber; // currently plyNumber's parity tells me whether it's white or black's move to save memory
		


	public:
		std::vector<Ply> legalMoves;

		BoardState();
		~BoardState();

		void generateLegalMoves();

		void makeMove(Ply ply);
		void undoMove(Ply ply);

		void setFromFEN(char* FEN);
		friend std::ostream& operator<< (std::ostream& os, const BoardState& board);
	};
}

