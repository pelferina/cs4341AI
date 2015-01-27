/*
 * IDDFS.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Ayesha
 */

#ifndef IDDFS_H_
#define IDDFS_H_

#include "main.h"

class IDDFS
{
private:
	int maxDepth;
	string move;
	gameBoard gameState;

public:
	IDDFS(gameBoard currState, int depth): maxDepth(depth), move(""), gameState(currState){
		run();
	}

	//Runs IDDFS
	void run();

	int getMaxDepth() const {
		return maxDepth;
	}

	void setMaxDepth(int maxDepth) {
		this->maxDepth = maxDepth;
	}

	const string& getMove() const {
		return move;
	}

	void setMove(const string& move) {
		this->move = move;
	}
};



#endif /* IDDFS_H_ */
