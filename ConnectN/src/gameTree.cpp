/*
 * gameTree.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

gameTree::gameTree(gameBoard headState, gameBoard testState): headGameState(headState), testBoard(testState), DepthSoFar(0)
{

}

gameNode* gameTree::buildTree(gameNode &startNode, int depth, miniOrMaxi miniOrMax)
{
	gameNode *bestNode = NULL;
	int count = 0;
	// If the depth is 0, calculate the heuristic value of the root Node
	if (depth == 0) {
		gameBoard currGameBoard(startNode.getBoardState());
		//Use the heuristic function to calculate its utility
		startNode.setUtilityValue(currGameBoard.heuristicEvaluator());
		cerr<<"\nutility value: "<<startNode.getUtilityValue()<<endl;
		bestNode = &startNode;
	}

	// If depth is 1, create children Nodes and update the bestNode
	else if (depth == 1) {
		gameBoard currGameBoard(startNode.getBoardState());

		for (int i = 0; i < currGameBoard.getWidth(); i++) {
			cerr<<"testing"<<endl;
			currGameBoard.printBoard();
			gameBoard popState(currGameBoard);
			gameBoard dropState(currGameBoard);
			if(miniOrMax == MAX)	//
			{
				dropState.setIsMyTurn(true);
				popState.setIsMyTurn(true);
			}
			else	//MIN
			{
				popState.setIsMyTurn(false);
				dropState.setIsMyTurn(false);
			}

			if (dropState.canDropADiscFromTop(i)) {
				cerr<<"\ndropping a disc "<< i <<"\n";
				dropState.dropADisc(i);
				gameNode *newNode = new gameNode(dropState, &startNode, i, startNode.getDepth() + 1);
				startNode.addAChild(newNode);
				count++;
			}
			if(popState.canRemoveADiscFromBottom(i)) {
				cerr<<"\npopping a disc "<< i <<"\n";
				popState.popOutADisc(i);
				gameNode *newNode = new gameNode(popState, &startNode, i, startNode.getDepth() + 1);
				startNode.addAChild(newNode);
				count++;
			}
		}
		startNode.removeEmptyChildren(count);
	}

	// If the depth is greater than 0, get the best Node at the current depth and perform MiniMax algorithm
	if (depth > 0) {
		gameNode *bestNodeTemp;
		vector<gameNode*>::const_iterator it = startNode.getChildrenNodes().begin();
		bool firstChildVisited = false;

		// For all child nodes, recursively do MiniMax
		while (it!=startNode.getChildrenNodes().end()) {
			// Get the next child Node
			gameNode nextChildNode((*it)->getBoardState());
			// Get the bestNode for the nextChildNode via recursion
			if(depth%2 == 0)	//Minimize
			{
				cerr<<"Minimizing "<< startNode.getChildrenNodes().size()<<"\n";
				bestNodeTemp = buildTree(nextChildNode, depth-1, MIN);
			}
			else	//Maximize
			{
				cerr<<"MAXimizing "<< startNode.getChildrenNodes().size()<<"\n";
				if(&nextChildNode!=NULL)
					cerr<<"child exists"<<endl;
				bestNodeTemp = buildTree(nextChildNode, depth-1, MAX);
			}

//			// Do Min/Max
//			if (!firstChildVisited) {
////				startNode.setUtilityValue(nextChildNode.getUtilityValue());
//				firstChildVisited = true;
//			}
//			//Maximize
//			else if (MAX && nextChildNode.getUtilityValue() > startNode.getUtilityValue()) {
////				startNode.setUtilityValue(nextChildNode.getUtilityValue());
//				bestNode = bestNodeTemp;
//			}
//			//Minimize
//			else if (MIN && nextChildNode.getUtilityValue() < startNode.getUtilityValue()) {
////				startNode.setUtilityValue(nextChildNode.getUtilityValue());
//				bestNode = bestNodeTemp;
//			}
//			//Alpha-Beta Pruning
//			if (startNode.getParent() != null) {
//				if ((MIN) && (startNode.getUtilityValue() < startNode.getParent().getUtilityValue())) {
//					startNode.clearChildren();
//					break;
//				}
//			}
			it++;
		}
	}
	return bestNode;
}
