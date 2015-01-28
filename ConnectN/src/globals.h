/*
 * globals.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

typedef struct Globals
{
	// Define cell patterns for empty, own and opponent's cell
	char emptyCell;
	char ownCell;
	char opponentCell;

	int IWIN;
	int OPPONENTWINS;
	int NOCONNECTION;
	int TIE;

	bool isPopOutLeft;
	bool timeoutReached;
}Globals;

typedef enum miniMax{ MAX, MIN, EMPTY }miniOrMaxi;

void initGlobals();
void timeout_handler(int sig);
//Used for dropping or popping gamePieces
#define DROP 1
#define POPOUT 0

//Used for gamePieces
#define PLAYER 1
#define OPPONENT 0
extern Globals globals;

#endif /* GLOBALS_H_ */
