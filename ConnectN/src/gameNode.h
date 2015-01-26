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
	vector<gameNode*> childrenNodes;
	int col;
    int depth;

public:
    gameNode(gameBoard state, gameNode *parent, int column, int depth);

    //Root node constructor
    gameNode(gameBoard state);

    void addAChild(gameNode *newNode);

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

	const gameNode* getParentNode() const {
		return parentNode;
	}

	const vector<gameNode*>& getChildrenNodes() const {
		return childrenNodes;
	}
};


#endif /* NODE_H_ */
