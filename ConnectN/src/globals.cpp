/*
 * globals.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha & Eric
 */

#include "globals.h"

void initGlobals()
{
	globals.emptyCell = '-';
	globals.ownCell = 'o';
	globals.opponentCell = 'x';

	globals.timeoutReached = false;
	globals.IWIN= 1;
	globals.OPPONENTWINS=2;
	globals.NOCONNECTION=-1;
	globals.TIE=0;

	globals.isPopOutLeft = true;
}

void timeout_handler(int sig)
{
	globals.timeoutReached = true;
}

