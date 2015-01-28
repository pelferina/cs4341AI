/*
 * gameTree.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha & Eric
 */

#include "main.h"

gameTree::gameTree(gameNode &headState)
{
	headGameState = &headState;
}

gameNode* gameTree::buildTree(gameNode &startNode, int depth, miniOrMaxi miniOrMax)
{
	gameNode *bestNode = NULL;
	int count = 0;
	// If the depth is 0, calculate the heuristic value of the given Node
	if (depth == 0) {
		gameBoard currGameBoard(startNode.getBoardState());
		//Use the heuristic function to calculate its utility
		startNode.setUtilityValue(currGameBoard.heuristicEvaluator());
//		cerr<<"\nutility value: "<<startNode.getUtilityValue()<<endl;
		bestNode = &startNode;
	}

	// If depth is 1, create children Nodes and update the bestNode
	else if (depth == 1) {

		gameBoard currGameBoard(startNode.getBoardState());
		for (int i = 0; i < currGameBoard.getWidth(); i++) {


			gameBoard popState(currGameBoard);
			gameBoard dropState(currGameBoard);

			if(miniOrMax == MAX)	//My turn
			{
				dropState.setIsMyTurn(true);
				popState.setIsMyTurn(true);
			}
			else	//Opponent's turn
			{
				popState.setIsMyTurn(false);
				dropState.setIsMyTurn(false);
			}

			if (dropState.canDropADiscFromTop(i)) {
//				DEBUG
//				cerr<<"testing"<<endl;
//				currGameBoard.printBoard();
//				cerr<<"\ndropping a disc "<< i <<"\n";
				dropState.dropADisc(i);
				//create move
				stringstream ss;
				ss << i;
				string move = ss.str();

				move = move + " 1\n";
				gameNode *newNode = new gameNode(dropState, &startNode, move, startNode.getDepth() + 1);
				startNode.addAChild(*newNode);
				count++;
			}
			if(popState.canRemoveADiscFromBottom(i)) {
//				DEBUG
//				cerr<<"testing"<<endl;
//				currGameBoard.printBoard();
//				cerr<<"\npopping a disc "<< i <<"\n";
				popState.popOutADisc(i);

				//create move
				stringstream ss;
				ss << i;
				string move = ss.str();

				move = move + " 0\n";
				gameNode *newNode = new gameNode(popState, &startNode, move, startNode.getDepth() + 1);
				startNode.addAChild(*newNode);
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
			gameNode *nextChildNode = (*it);
			// Get the bestNode for the nextChildNode via recursion
			if(miniOrMax == MAX)	//Minimize
			{

				bestNodeTemp = buildTree(*nextChildNode, depth-1, MIN);
//				if(bestNodeTemp != NULL)
//					cerr<<"\nNOT NULL\n";
			}
			else	//Maximize
			{

				bestNodeTemp = buildTree(*nextChildNode, depth-1, MAX);
//				if(bestNodeTemp != NULL)
//					cerr<<"\nNOT NULL\n";
			}
//			DEBUG
//			cerr<<"child : "<<nextChildNode->getUtilityValue()<<" start: " << startNode.getUtilityValue()<<endl;
//			cerr<<"start:"<<startNode.getUtilityValue()<<endl;
			// Do Min/Max
			if (!firstChildVisited) {

				startNode.setUtilityValue(nextChildNode->getUtilityValue());

				firstChildVisited = true;
			}
			//Maximize
			else if (miniOrMax == MAX && nextChildNode->getUtilityValue() > startNode.getUtilityValue()) {
//				cerr<<"MAXimizing "<< startNode.getChildrenNodes().size()<<"\n";
				startNode.setUtilityValue(nextChildNode->getUtilityValue());
				bestNode = bestNodeTemp;
			}
			//Minimize
			else if (miniOrMax == MIN && nextChildNode->getUtilityValue() < startNode.getUtilityValue()) {
//				cerr<<"Minimizing "<< startNode.getChildrenNodes().size()<<"\n";
				startNode.setUtilityValue(nextChildNode->getUtilityValue());
				bestNode = bestNodeTemp;
			}
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
//	cerr<<"\nreturning bestnode\n";
	return bestNode;
}

void gameTree::printTree(int n)
{
//	cerr<<"\n"<<headGameState->getUtilityValue();
//	cerr<<"\n";
//	if(n>=1)
//	{
//		for(vector<gameNode*>::const_iterator it = headGameState->getChildrenNodes().begin(); it!= headGameState->getChildrenNodes().end(); it++)
//		{
//			cerr<<(*it)->getUtilityValue()<< " ";
//		}
//		cerr<<"\n";
//	}
//	if(n>=2)
//	{
//		for(vector<gameNode*>::const_iterator it = headGameState->getChildrenNodes().begin(); it!= headGameState->getChildrenNodes().end(); it++)
//		{
//			for(vector<gameNode*>::const_iterator it2 = (*it)->getChildrenNodes().begin(); it2!=(*it)->getChildrenNodes().end(); it2++)
//			{
//				cerr<<(*it2)->getUtilityValue()<<" ";
//			}
//			cerr<<" - ";
//		}
//		cerr<<"\n";
//	}
//	if(n>=3)
//	{
//		for(vector<gameNode*>::const_iterator it = headGameState->getChildrenNodes().begin(); it!= headGameState->getChildrenNodes().end(); it++)
//		{
//			for(vector<gameNode*>::const_iterator it2 = (*it)->getChildrenNodes().begin(); it2!=(*it)->getChildrenNodes().end(); it2++)
//			{
//				for(vector<gameNode*>::const_iterator it3 = (*it2)->getChildrenNodes().begin(); it3!=(*it2)->getChildrenNodes().end(); it3++)
//				{
//					cerr<<(*it3)->getUtilityValue()<<" ";
//				}
//				cerr<<" - ";
//			}
//			cerr<<" - ";
//		}
//		cerr<<"\nend";
//	}
}
