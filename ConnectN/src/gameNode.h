/*
 * node.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef NODE_H_
#define NODE_H_

#include "main.h"

class gameNode {
private:
	gameNode *parentNode;
	float utilityValue;
	gameBoard boardState;

	string move;
    int depth;

public:
    vector<gameNode*> childrenNodes;
    gameNode(gameBoard state, gameNode *parent, string move, int depth);

    //Root node constructor
    gameNode(gameBoard state);

    void addAChild(gameNode &newNode);

    void removeEmptyChildren(int count);

	const gameBoard& getBoardState() const {
		return boardState;
	}

	int getDepth() const {
		return depth;
	}

	void setDepth(int depth) {
		this->depth = depth;
	}

	float getUtilityValue() const {
		return utilityValue;
	}

	void setUtilityValue(float utilityValue) {
		this->utilityValue = utilityValue;
	}

	gameNode* getParentNode()  {
		return parentNode;
	}

	const vector<gameNode*>& getChildrenNodes() const {
		return childrenNodes;
	}

	const string& getMove() const {
		return move;
	}
};


#endif /* NODE_H_ */
