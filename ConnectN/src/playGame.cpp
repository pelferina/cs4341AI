/*
 * playGame.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

// Initialize Game
playGame::playGame(int playerNum, int h, int w, int N, int turn, int time): isNegative(false), superGameBoard(w, h, N)
{
	playerNumber = playerNum;
	height = h;
	width = w;
	numToWin = N;
	playerTurn = turn;
	timeLimit = time;
	setMyTurn((playerTurn == playerNumber) ? true : false);
}

// Start the Game
void playGame::startGame()
{
	cin.ignore(100,'\n'); //Ignores the newline character from cin in readGameConfig

	while (true) {
			if ( getMyTurn() ) {
					globals.timeoutReached = false;
					signal(SIGALRM, timeout_handler);	// Set Signal Handler
					alarm(timeLimit);			// Start the Timer

					playerMove = processOwnTurn(this->superGameBoard);

					this->superGameBoard.updateBoard(getMyTurn(), playerMove);

					cout << playerMove << endl;	// Send Move to Referee
					cout.flush();
			} else {
					processOpponentsTurn(); // Read Opponent's Move
					this->superGameBoard.updateBoard(getMyTurn(), opponentMove);
					if (isNegative)		// Check for End of Game
						break;
			}
			this->superGameBoard.printBoard();
			setMyTurn(!getMyTurn());	// Switch turns between moves

	}
}

string playGame::processOwnTurn(gameBoard gameState)
{
		if ( globals.timeoutReached ) {	// check for timeout
			cerr<<"timeout\n";	// ... TODO: Cut off test
		} else {
			gameTree tree(gameState, gameState);
			miniOrMaxi m = MAX;
			gameNode startNode(gameState);
			tree.buildTree(startNode,1, m);
			tree.buildTree(startNode,2, m);
//			tree.buildTree(new gameNode(gameState),2, m);
				// TODO: minimax iterative search
		}
		//Simply drop discs in column 1 for communication testing purposes
		return "0 1";
}

// Process Opponent's turn
void playGame::processOpponentsTurn()
{
	string opponentsState;

	getline(std::cin, opponentsState);	// Receive opponent's move information from Referee

	std::stringstream stream(opponentsState);	// Counts the words received
	int wordCount = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());

	switch(wordCount)
	{
		case 1:	// -1 indicating that the player won the game;
				// -2 indicating that the player lost the game; and
				// -3 indicating a tie.
				endOfGame = atoi(opponentsState.c_str());
				isNegative = true;
				break;
		case 2: // Valid Opponent Move
				opponentMove = opponentsState;
				break;
		default: //cerr << "Error in receive";
				break;
	}
}



