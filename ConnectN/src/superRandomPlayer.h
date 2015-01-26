/*
 * superRandomPlayer.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha
 */

#ifndef SUPERRANDOMPLAYER_H_
#define SUPERRANDOMPLAYER_H_

using namespace std;

/*
 * The Player Class
 */
class superRandomPlayer
{

private:

	// Declare Player Variables
	string playerName;
	int playerNumber;

public:

	// Initialize Game State
	superRandomPlayer(string name);

	// Receive Player Number from Format such as "player1: aa player2: bb"
	void initPlayer();

	/*
	 * Getters and Setters
	 */
	const string& getPlayerName() const {
		return playerName;
	}

	int getPlayerNumber() const {
		return playerNumber;
	}

};




#endif /* SUPERRANDOMPLAYER_H_ */
