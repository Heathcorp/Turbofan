#include "MoveTree.h"

namespace Turbofan
{
	MoveTree::Node::Node()
	{
		parent = nullptr;

		lastPly = { 0, 0, 0 };
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
}