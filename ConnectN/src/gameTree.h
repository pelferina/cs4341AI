/*
 * gameTree.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef GAMETREE_H_
#define GAMETREE_H_

#include "main.h"

/*
 * Tree implementing Depth-First Mini-Max algorithm
 */
class gameTree
{
private:
	gameNode headGameState;
	gameBoard testBoard;
	int DepthSoFar;
public:
	gameTree(gameBoard headState, gameBoard testState);

	gameNode* buildTree(gameNode &startNode, int depth, miniOrMaxi miniOrMax);



};


#endif /* GAMETREE_H_ */
