/*
 * superRandomPlayer.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#include "main.h"

// Initialize Game State
superRandomPlayer::superRandomPlayer(string name) {
	playerName = name;
	initPlayer();
}

// Receive Player Number from Format such as "player1: aa player2: bb"
void superRandomPlayer::initPlayer()
{
	cout << playerName << endl;	// Send playerName to Referee

	string playerInformation;

	getline(std::cin, playerInformation);	// Receive player number from Referee

	int found = playerInformation.find(playerName);	// Parse the information
	playerNumber = atoi(&playerInformation[found-3]);	// Get the player number in integer type
}
