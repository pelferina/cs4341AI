/*
 * gamePiece.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef GAMEPIECE_H_
#define GAMEPIECE_H_
#include "main.h"
//Game piece class. Stores location and owner.
class GamePiece{
public:
	int col, row; //Location of the piece in column and row.
	bool isPlayers; //true or false that it is the player's piece. PLAYER = true, OPPONENT =false
	GamePiece(int r, int c, int p){
		row=r;
		col=c;
		if(p==PLAYER)
			isPlayers=true;
		else
			isPlayers=false;
	}
};



#endif /* GAMEPIECE_H_ */
