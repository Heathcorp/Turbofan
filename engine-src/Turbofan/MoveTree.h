#include "BoardState.h"

#pragma once
namespace Turbofan
{

	class MoveTree
	{
	private:
		BoardState initialState;
	public:
		class Node
		{
		private:
			Node* parent;
			std::vector<Node*> children;

			Ply lastPly;

			//bool locked;
		public:
			Node();
			Node(Node* parentNode, Ply ply);
			~Node();

			Node* AddChild(Ply ply);
		};

		Node* rootNode;
	public:
		MoveTree(BoardState startingBoard);
		~MoveTree();
	};
}