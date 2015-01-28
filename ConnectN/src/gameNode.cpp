/*
 * node.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

gameNode::gameNode(gameBoard state, gameNode *parent, string mov, int depth): utilityValue(0), boardState(state), move(mov), depth(0)
{
//	cerr<<"\n creating a new node\n";
	this->parentNode = parent;
}

//Root node constructor
gameNode::gameNode(gameBoard state):utilityValue(0), boardState(state), move(" "), depth(0)
{
//	cerr<<"\n creating a root node\n";
	this->parentNode = NULL;
}

void gameNode::addAChild (gameNode &node)
{
	childrenNodes.push_back(&node);
	gameBoard currGameState = node.getBoardState();
//	currGameState.printBoard();
}

void gameNode::removeEmptyChildren(int count)
{
	childrenNodes.erase(childrenNodes.begin(), childrenNodes.begin()+(childrenNodes.size()-count));
}
