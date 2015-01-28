/*
 * game.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef GAME_H_
#define GAME_H_

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iterator>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <list>
#include <map>
#include <fstream>

using namespace std;

class gameBoard
{
private:
	/* Declare Game Board Variables	 */
	vector< vector < char > > board;
	int width, height, numToWin;
	bool isMyTurn;
	vector<int> numDiscsInColumn;

	int checkHorizontally();
	int checkVertically();
	int checkDiagonally1();
	int checkDiagonally2();
	bool isFull();
public:

	gameBoard(int w, int h, int N);
	gameBoard(const gameBoard &anotherBoard);

	//Lists of pieces
	vector<GamePiece> playerPieces;
	vector<GamePiece> opponentPieces;

	int isConnectN();
	void updateBoard(bool turn, string move);
	void dropADisc(int col);
	void popOutADisc(int col);
	void printBoard();
	bool canRemoveADiscFromBottom(int col);
	bool canDropADiscFromTop(int col);

	const vector<vector<char> >& getBoard() const {
		return board;
	}

	bool getIsMyTurn() const {
		return isMyTurn;
	}

	void setIsMyTurn(bool isMyTurn) {
		this->isMyTurn = isMyTurn;
	}

	int getHeight() const {
		return height;
	}


	const vector<int>& getNumDiscsInColumn() const {
		return numDiscsInColumn;
	}

	int getNumToWin() const {
		return numToWin;
	}
	void setBoard(const vector<vector<char> >& board) {
			this->board = board;
	}
	int getWidth() const {
		return width;
	}

	//Some prototypes
	map<int,int> comboCounter(int activePl);
	map<int,int> checkHorizontal(int activePl);
	map<int,int> checkVertical(int activePl);
	map<int,int> checkDiagonalNE(int activePl);
	map<int,int> checkDiagonalSE(int activePl);
	//Calculates heuristics based on weighted values for each number of combos
	double heuristicEvaluator();

};

#endif /* GAME_H_ */
