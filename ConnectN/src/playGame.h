/*
 * playGame.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "main.h"

class playGame
{
private:

	// Declare game variables
	bool isMyTurn, isNegative;
	int width, height, numToWin, playerNumber, playerTurn, timeLimit, endOfGame;
	string playerNameInformation, playerMove, opponentMove, gameState;
	gameBoard superGameBoard;

public:

	// Initialize Game
	playGame(int playerNum, int h, int w, int N, int turn, int time);

	// Start the Game
	void startGame();

	//Iterative-deepening depth-first search move generator
	string IDDFS(gameBoard gameState, int depth);

	//Process own turn
	string processOwnTurn(gameBoard gameState);

	// Process Opponent's turn
	void processOpponentsTurn();

	/*
	 * Getters and Setters
	 */
	bool getMyTurn()
	{
		return this->isMyTurn;
	}

	void setMyTurn(bool turn)
	{
		this->isMyTurn = turn;
	}
};




#endif /* PLAYGAME_H_ */
