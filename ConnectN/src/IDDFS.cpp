/*
 * IDDFS.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Ayesha
 */

#include "main.h"

// Performs Iterative Deeepening Depth-First Search
void IDDFS::run()
{
	gameNode startNode(gameState);	//Creates a head node for the given state
	gameTree tree(startNode);	//Creates a tree with the given head node

	miniOrMaxi m = MAX;
	for(int i = 1; i <= maxDepth; i++)	// Iteratively increments depth for search
	{
		tree.buildTree(startNode, i, m);		//Returns best node within search

		for(vector<gameNode*>::const_iterator it = tree.getHeadGameState()->getChildrenNodes().begin(); it!= tree.getHeadGameState()->getChildrenNodes().end(); it++)
		{
			if((*it)->getUtilityValue()==(tree.getHeadGameState()->getUtilityValue()))
				move = (*it)->getMove();	//returns best move for the search
		}
		tree.printTree(i);

	}
}

