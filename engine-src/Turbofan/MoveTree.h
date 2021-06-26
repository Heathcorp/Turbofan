#include "BoardState.h"

#pragma once
namespace Turbofan
{

	class MoveTree
	{
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

	private:
		Node* rootNode;
		BoardState initialState;
	public:
		MoveTree(BoardState startingBoard);
		~MoveTree();
	};
}