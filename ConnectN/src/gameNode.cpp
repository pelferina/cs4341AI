/*
 * node.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

gameNode::gameNode(gameBoard state, gameNode *parent, int column, int depth): utilityValue(0), boardState(state), childrenNodes(2*boardState.getWidth()), col(column), depth(0)
{
	this->parentNode = parent;
}

//Root node constructor
gameNode::gameNode(gameBoard state):utilityValue(0), boardState(state), childrenNodes(2*boardState.getWidth()), col(-1), depth(0)
{
	this->parentNode = NULL;
}

void gameNode::addAChild (gameNode *node)
{
	childrenNodes.push_back(node);
	gameBoard currGameState = node->getBoardState();
	currGameState.printBoard();
}

void gameNode::removeEmptyChildren(int count)
{
	childrenNodes.erase(childrenNodes.begin(), childrenNodes.begin()+(childrenNodes.size()-count));
}
