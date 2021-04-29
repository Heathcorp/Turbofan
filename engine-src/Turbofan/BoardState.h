#pragma once
namespace Turbofan
{
	typedef unsigned long long uint64;
	typedef unsigned char uint8;

	class BoardState
	{
	private:
		struct PlayerPieces
		{
			struct King
			{
				uint8 index : 6;
				bool canCastleK : 1;
				bool canCastleQ : 1;
			} king;
			uint64 queen, rook, bishop, knight, pawn;
		};
		// kings, queens, rooks, bishops, knights and pawns for each colour;

		PlayerPieces white, black;


	public:
		BoardState();
		void setFromFEN(char* FEN);



		~BoardState();
	};
}

