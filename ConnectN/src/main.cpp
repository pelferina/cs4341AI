/*
 * main.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

Globals globals;

int main(int argc, char **argv)
{
		initGlobals();

		superRandomPlayer myPlayer("srl");	// Creates our super random player with given name

		ofstream myfile;
		myfile.open ("debug.txt");

		int height, width, numToWin, playerTurn, timeLimit;

		// Receive game config board as the following 5 numbers, in order:
		// height (#rows), board width (#columns),
		// number of pieces to win (the N in Connect-N), turn of the player
		// and the time limit to make a move in seconds.

		cin >> height >> width >> numToWin >> playerTurn >> timeLimit;
//		DEBUG FILE
		myfile << "Writing this to a file.\n";
		myfile<<height << " " << width<< " " << numToWin << " " << playerTurn << " " << timeLimit<<endl;
		myfile.close();

		playGame newGame(myPlayer.getPlayerNumber(), height, width, numToWin, playerTurn, timeLimit);	// Initializes game variables

		newGame.startGame();	// Begins game play



        return 0;
}



