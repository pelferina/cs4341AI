/*
 * gameBoard.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Ayesha & Eric
 */

#include "main.h"

//Initializes game board
gameBoard::gameBoard(int w, int h, int N): board(h, vector < char >(w, (globals.emptyCell))), numDiscsInColumn(w, 0)
{
	width = w;
	height = h;
	numToWin = N;
	printBoard();
}
// Copy constructor
gameBoard::gameBoard(const gameBoard &anotherBoard)
{
	this->board = anotherBoard.getBoard();
	this->height = anotherBoard.getHeight();
	this->width = anotherBoard.getWidth();
	this->numToWin = anotherBoard.getNumToWin();
	this->isMyTurn = anotherBoard.getIsMyTurn();
	this->numDiscsInColumn = anotherBoard.getNumDiscsInColumn();
	this->opponentPieces = anotherBoard.opponentPieces;
	this->playerPieces = anotherBoard.playerPieces;
}
// updates board every turn
void gameBoard::updateBoard(bool turn, string move)
{
	isMyTurn = turn;

	int col = 0;
	int type = 0;

	std::istringstream iss(move); // Split move into column and type of disc
	std::string token;
	int i = 0;
	while(getline(iss, token, ' '))
	{
		if ( i == 0 )
		{
			col = atoi(token.c_str());
			i++;
		}
		else if ( i == 1)
			type = atoi(token.c_str());
	}

	if(type == DROP)	//Drops the disc in the col
		dropADisc(col);
	else
	{
		popOutADisc(col);	//Pops the disc from the col
		globals.isPopOutLeft = false;	// Makes sure that pop out is only used once (Used as a check in heuristic evaluator)
	}
}

//Drops a disc
void gameBoard::dropADisc(int col)
{
	int firstEmptyCellRow = 0;
	firstEmptyCellRow = height - numDiscsInColumn[col] - 1;
	if(isMyTurn)
	{
//		cerr<<"my turn"<<endl;
		board[firstEmptyCellRow][col] = globals.ownCell;

		//Add game piece to player's list
		GamePiece g(firstEmptyCellRow, col, PLAYER);
		playerPieces.push_back(g);
	}
	else
	{
//		cerr<<"opp turn"<<endl;
		board[firstEmptyCellRow][col] = globals.opponentCell;

		//Add game piece to opponent's list
		GamePiece g(firstEmptyCellRow, col, OPPONENT);
		opponentPieces.push_back(g);
	}
	this->numDiscsInColumn[col]++;

}

// Pops out a disc
void gameBoard::popOutADisc(int col)
{
	int i;
	for(i = height-1 ; i > height-this->numDiscsInColumn[col]; i--){
		board[i][col]=board[i-1][col];
	}
	board[i][col] = globals.emptyCell;
	this->numDiscsInColumn[col]--;
}

//Prints board
void gameBoard::printBoard()
{
	std::vector< std::vector<char> >::const_iterator row;
	std::vector<char>::const_iterator col;
	cerr<< "Board:\n";
	for (row = board.begin(); row != board.end(); ++row)
	{
		 for ( col = row->begin(); col != row->end(); ++col)
		 {
			cerr << *col <<" ";
		 }
		 cerr<<"\n";
	}


}

bool gameBoard::canRemoveADiscFromBottom(int col){

//		cerr<<"checking popping: "<< board[height-1][col]<< " " << globals.ownCell<< " " <<endl;
	if(col<0 || col>=this->width) {
//		cerr<<"Illegal column!"<<endl;
		return false;
	}
	else if(getIsMyTurn() && board[height-1][col]!=globals.ownCell){
//		cerr<<"You don't have a checker in column " << col << " to pop out!"<<endl;
		return false;
	}
	else if(!getIsMyTurn() && board[height-1][col]!=globals.opponentCell)
	{
//		cerr<<"You don't have a checker in column " << col << " to pop out!"<<endl;
		return false;
	}
	else
		return true;
}

bool gameBoard::canDropADiscFromTop(int col)
{
//	cerr<<"\ncan drop a disc\n";
	if(col<0 || col>=this->width) {
//		cerr<< "Illegal column!"<<endl;
		return false;
	}
	else if(this->numDiscsInColumn[col]==this->height){
//		cerr<<"Column is already full. Cannot drop more disc in it."<<endl;
		return false;
	}
	else
	{
//		cerr<<numDiscsInColumn.size()<<numDiscsInColumn[col];
//		cerr<<"dropping a disc cause its true";
		return true;
	}

}

/**
* Check if one of the players gets N checkers in a row (horizontally, vertically or diagonally)
*  @return the value of winner. If winner=-1, nobody win and game continues; If winner=0/TIE, it's a tie;
* 			If winner=1, player1 wins; If winner=2, player2 wins.
*/

int gameBoard::isConnectN(){

	bool tmp_winner = checkHorizontally();

	if(tmp_winner!=globals.NOCONNECTION)
		return tmp_winner;

	 tmp_winner=checkVertically();
	 if(tmp_winner!=globals.NOCONNECTION)
			return tmp_winner;

	 tmp_winner=checkDiagonally1();
	 if(tmp_winner!=globals.NOCONNECTION)
			return tmp_winner;
	 tmp_winner=checkDiagonally2();
	 if(tmp_winner!=globals.NOCONNECTION)
			return tmp_winner;

	 return globals.NOCONNECTION;

}

int gameBoard::checkHorizontally()
{
	int max1=0;
	int max2=0;
	bool iWin=false;
	bool opponentWins=false;

	//check each row, horizontally
	for(int i=0;i<this->height;i++){
		max1=0;
		max2=0;
		for(int j=0;j<this->width;j++){
			if(board[i][j]==globals.ownCell){
				max1++;
				max2=0;
				if(max1==numToWin)
					iWin=true;
			}
			else if(board[i][j]==globals.opponentCell){
				max1=0;
				max2++;
				if(max2==numToWin)
					opponentWins=true;
			}
			else{
				max1=0;
				max2=0;
			}
		}
	}
	if (opponentWins && iWin)
	 return globals.TIE;
	if (opponentWins)
	 return globals.OPPONENTWINS;
	if (iWin)
	 return globals.IWIN;

	return globals.NOCONNECTION;
}

int gameBoard::checkVertically(){
	//check each column, vertically
	int max1=0;
	int max2=0;
	bool iWin=false;
	bool opponentWins=false;

	 for(int j=0;j<this->width;j++){
		 max1=0;
		 max2=0;
		for(int i=0;i<this->height;i++){
			if(board[i][j]==globals.ownCell){
				max1++;
				max2=0;
				if(max1==numToWin)
					iWin=true;
			}
			else if(board[i][j]==globals.opponentCell){
				max1=0;
				max2++;
				if(max2==numToWin)
					opponentWins=true;
			}
			else{
				max1=0;
				max2=0;
			}
		}
	 }
	 if (iWin && opponentWins)
		 return globals.TIE;
	 if (iWin)
		 return globals.IWIN;
	 if (opponentWins)
		 return globals.OPPONENTWINS;

	 return globals.NOCONNECTION;
}

int gameBoard::checkDiagonally1(){
	//check diagonally y=-x+k
	int max1=0;
	int max2=0;
	bool iWin=false;
	bool opponentWins=false;

	int upper_bound=height-1+width-1-(numToWin-1);

	for(int k=numToWin-1;k<=upper_bound;k++){
		max1=0;
		max2=0;
		int x,y;
		if(k<width)
			x=k;
		else
			x=width-1;
		y=-x+k;

		while(x>=0  && y<height){
			// System.out.println("k: "+k+", x: "+x+", y: "+y);
			if(board[height-1-y][x]==globals.ownCell){
				max1++;
				max2=0;
				if(max1==numToWin)
					iWin=true;
			}
			else if(board[height-1-y][x]==globals.opponentCell){
				max1=0;
				max2++;
				if(max2==numToWin)
					opponentWins=true;
			}
			else{
				max1=0;
				max2=0;
			}
			x--;
			y++;
		}

	}
	 if (iWin && opponentWins)
		 return globals.TIE;
	 if (iWin)
		 return globals.IWIN;
	 if (opponentWins)
		 return globals.OPPONENTWINS;

	 return globals.NOCONNECTION;
}

int gameBoard::checkDiagonally2(){
	//check diagonally y=x-k
	int max1=0;
	int max2=0;
	bool iWin=false;
	bool opponentWins=false;

	int upper_bound=this->width-1-(numToWin-1);
	int  lower_bound=-(height-1-(numToWin-1));
	// System.out.println("lower: "+lower_bound+", upper_bound: "+upper_bound);
	 for(int k=lower_bound;k<=upper_bound;k++){
		 max1=0;
		 max2=0;
		 int x,y;
		 if(k>=0)
			 x=k;
		 else
			 x=0;
		 y=x-k;
		while(x>=0 && x<width && y<height){
			// System.out.println("k: "+k+", x: "+x+", y: "+y);
			if(board[height-1-y][x]==globals.ownCell){
				max1++;
				max2=0;
				if(max1==numToWin)
					iWin=true;
			}
			else if(board[height-1-y][x]==globals.opponentCell){
				max1=0;
				max2++;
				if(max2==numToWin)
					opponentWins=true;
			}
			else{
				max1=0;
				max2=0;
			}
			x++;
			y++;
		}

 }	 //end for y=x-k

	 if (iWin && opponentWins)
		 return globals.TIE;
	 if (iWin)
		 return globals.IWIN;
	 if (opponentWins)
		 return globals.OPPONENTWINS;

	 return globals.NOCONNECTION;
}

bool gameBoard::isFull(){
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++){
			if(board[i][j]==globals.emptyCell)
				return false;
		}
	return true;
}

//Calculates heuristics based on weighted values for each number of combos
double gameBoard::heuristicEvaluator(){

	//TESTING
	int n = getNumToWin(); //Number of pieces needed to win

	double hVal = 0;

	//Calculate player and opponent line combos
	map<int,int> Lo = comboCounter(OPPONENT); //Number of line combos of each length 2 to n
	map<int,int> Lp = comboCounter(PLAYER); //Number of line combos of each length

	if(Lo[n] >= 1 && Lp[n] >= 1){
		return 10000; //If both opponent and player have multiple winds, it would be a tie. This AI decides ties are better than normal moves, but worse than wins
	}else if(Lo[n] >= 1){
		return -1; //If opponent has a line combo of n or more, that is a loss. Worst choice
	}else if(Lp[n] >= 1){
		return 15000; //If player has line combo of n or more, that is a win. Best choice!
	}

	double oval=0.001;
	for(int i=2; i<n; i++){
		oval += Lo[i] *i*i;
	}

	double pval=0.001;
	for(int i=2; i<n; i++){
		pval += Lp[i] *i*i;
	}

	hVal = pval / oval;

	//Real return

	return hVal;

	//TESTING - returns between 0 and 1 just for testing purposes
//	return rand()%100;


//	cerr << "pval / oval :"<<pval << "/" <<oval<<"\n";
}

map<int,int> gameBoard::comboCounter(int activePl){

	map<int,int> combos;

	//Get combos from every direction
	//Add horizontal combos
	combos = checkHorizontal(activePl);

	//add vertical combos
	map <int, int> VCombos = checkVertical(activePl);
	for (map<int,int>::iterator it=VCombos.begin(); it!=VCombos.end(); ++it){
		combos[it->first] += VCombos[it->first];
	}

	//Add diagonal combos
	map <int, int> DNCombos = checkDiagonalNE(activePl);
	for (map<int,int>::iterator it=DNCombos.begin(); it!=DNCombos.end(); ++it){
		combos[it->first] += DNCombos[it->first];
	}

	//Add the other diagonal
	map <int, int> DSCombos = checkDiagonalSE(activePl);
	for (map<int,int>::iterator it=DSCombos.begin(); it!=DSCombos.end(); ++it){
		combos[it->first] += DSCombos[it->first];
	}

	//Pointer towards the array of combos
	return combos;
}

//Counts the number and combo size, of horizontal combos on the board
map<int,int> gameBoard::checkHorizontal(int activePl){

	int n= getNumToWin(); //N - goal size
	int width=getWidth(); //Width of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 = playerPieces;
		p2 = opponentPieces;
	}else{
		p1 = opponentPieces;
		p2 = playerPieces;
	}

	map<int, int> combos; //Stores combo numbers

	//Loop through active player's pieces
	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;


		//Check left of the piece
		int leftRange = c-(n-1);
		if(c-(n-1) < 0){ //Left edge found
			leftRange = 0;
		}
		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){
			if(r==p2[j].row){
				if((p2[j].col >= leftRange) && (p2[j].col < c)){ //enemy piece interposed
					leftRange = p2[j].col+1;
				}
			}
		}
		//End of left check

		//Check right of the piece
		int rightRange = c+(n-1);
		if(c+(n-1) > width-1){ //Right edge found
			rightRange = width-1;
		}
		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){
			if(r==p2[j].row){
				if((p2[j].col <= rightRange) && (p2[j].col > c)){
					rightRange = p2[j].col-1;
				}
			}
		}
		//End of right check

		//See if combos are possible
		if(rightRange - leftRange +1 < n){ //Range is not big enough to score a win
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length raher than potential length

			//checking left
			for(int j=c-1;j>=leftRange;j--){
				bool found=false;
				for(unsigned k=0; k<p1.size();k++){ //Cycle through all friendly pieces
					if(p1[k].row==r){
						if(p1[k].col == j){ //Is this friendly piece in the right spot?
							found=true;
						}
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					leftRange=j;
					break;
				}
			}

			//checking right
			for(int j=c+1;j<=rightRange; j++){
				bool found=false;
				for(unsigned k=0; k<p1.size(); k++){
					if(p1[k].col == j && p1[k].row == r){ //Is this friendly piece in the right spot?
						found=true;
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					rightRange=j;
					break;
				}
			}
			//Finished checking
			int comboLength = rightRange - leftRange +1;
			if(combos.find(comboLength) != combos.end()){
				combos[comboLength] = combos[comboLength] +1; //Increase number of that combo length
			}else{
				combos[comboLength] =1; //one has been found of this length
			}
		}
	}//End loop of pieces
	return combos;
}

//Counts the number and combo size, of horizontal combos on the board
map<int,int> gameBoard::checkVertical(int activePl){

	int n= getNumToWin(); //N - goal size
	int height=getHeight(); //Width of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 =  playerPieces;
		p2 =  opponentPieces;
	}else{
		p1 =  opponentPieces;
		p2 =  playerPieces;
	}

	map<int, int> combos; //Stores combo numbers

	//Loop through active player's pieces
	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;


		//Check below the piece
		int bottomRange = c-(n-1);
		if(c-(n-1) < 0){ //Bottom edge found
			bottomRange = 0;
		}
		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){
			if(r==p2[j].col){
				if((p2[j].row >= bottomRange) && (p2[j].row < c)){ //enemy piece interposed
					bottomRange = p2[j].row+1;
				}
			}
		}
		//End of below check

		//Check above the piece
		int topRange = c+(n-1);
		if(c+(n-1) > height-1){ //above edge found
			topRange = height-1;
		}
		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){
			if(r==p2[j].col){
				if((p2[j].row <= topRange) && (p2[j].row > c)){
					topRange = p2[j].row-1;
				}
			}
		}
		//End of above check

		//See if combos are possible
		if(topRange - bottomRange +1 < n){ //Range is not big enough to score a win
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length raher than potential length

			//checking bottom
			for(int j=r-1;j>=bottomRange;j--){
				bool found=false;
				for(unsigned k=0; k<p1.size();k++){ //Cycle through all friendly pieces
					if(p1[k].col==c){
						if(p1[k].row == j){ //Is this friendly piece in the right spot?
							found=true;
						}
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					bottomRange=j;
					break;
				}
			}

			//checking top
			for(int j=c+1;j<=topRange; j++){
				bool found=false;
				for(unsigned k=0; k<p1.size(); k++){
					if(p1[k].col == c && p1[k].row == j){ //Is this friendly piece in the right spot?
						found=true;
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					topRange=j;
					break;
				}
			}
			//Finished checking
			int comboLength = topRange - bottomRange +1;
			if(combos.find(comboLength) != combos.end()){
				combos[comboLength] = combos[comboLength] +1; //Increase number of that combo length
			}else{
				combos[comboLength] =1; //one has been found of this length
			}
		}
	}//End loop of pieces
	return combos;
}

//Counts the number and combo size, of diagonal combos on the board
//Diagonal from 0,0 to x,x /
map<int,int> gameBoard::checkDiagonalNE(int activePl){

	int n=  getNumToWin(); //N - goal size
	int width= getWidth(); //Width of game board
	int height= getHeight(); //Height of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 =  playerPieces;
		p2 =  opponentPieces;
	}else{
		p1 =  opponentPieces;
		p2 =  playerPieces;
	}

	map<int, int> combos; //Stores combo numbers

	//Loop through active player's pieces
	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;


		//Check left of the piece
		int leftRangeColumn = c-(n-1);
		if(c-(n-1) < 0){ //Left column edge found
			leftRangeColumn = 0;
		}
		int leftRangeRow = r-(n-1);
		if(r-(n-1) > height-1){ //Left row edge found
			leftRangeRow = height-1;
		}

		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){

			if(p2[j].row >= leftRangeRow && p2[j].row < r){ //above piece
				if((p2[j].col >= leftRangeColumn) && (p2[j].col < c)){ //left of piece
					//In the range of blocking

					int val = r-leftRangeRow; //Only check in the non blocked range

					//count up to the determined val looking for blocking pieces
					for(int q=1; q<=val; q++){
						if(p2[j].row == r-q && p2[j].col == r-q){
							leftRangeColumn = p2[j].col+1; //potential range now lowered
							leftRangeRow = p2[j].row+1;

							val = r-leftRangeRow; //Update the value

						}
					}
				}
			}
		}
		//End of left check

		//CHECK RIGHT
		//Check right of the piece
		int rightRangeColumn = c+(n-1);
		if(c+(n-1) > width-1){ //right column edge found
			rightRangeColumn = width-1;
		}
		int rightRangeRow = r+(n-1);
		if(r+(n-1) < 0){ //right row edge found
			rightRangeRow = 0;
		}

		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){

			if(p2[j].row <= rightRangeRow && p2[j].row > r){
				if((p2[j].col <= rightRangeColumn) && (p2[j].col > c)){ //enemy piece in area of blocking
					//In the range of blocking

					int val = rightRangeColumn-c; //determine how many spaces to search

					//count up to the determined val looking for
					for(int q=1; q<=val; q++){
						if(p2[j].row == r+q && p2[j].col == r+q){
							rightRangeColumn = p2[j].col-1;
							rightRangeRow = p2[j].row-1;

							//reduce the range again
							val = rightRangeColumn-c;

						}
					}
				}
			}
		}
		//End of right check

		//See if combos are possible
		//If column OR row are n or smaller, then there is not enough space.
		if(rightRangeColumn - leftRangeColumn +1 < n){ //Range is not big enough to score a win.
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length rather than potential length

			//checking left
			for(int j=1;j<=c-leftRangeColumn;j++){
				bool found=false;
				for(unsigned k=0; k<p1.size();k++){ //Cycle through all friendly pieces
					if(p1[k].row==r-j){
						if(p1[k].col == c-j){ //Is this friendly piece in the right spot?
							found=true;
						}
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					leftRangeColumn=c-j+1;
					leftRangeRow=r-j+1;
					break;
				}
			}

			//checking right
			for(int j=1;j<=rightRangeColumn-c; j++){
				bool found=false;
				for(unsigned k=0; k<p1.size(); k++){
					if(p1[k].col == c+j && p1[k].row == r+j){ //Is this friendly piece in the right spot?
						found=true;
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					rightRangeColumn=c+j-1;
					rightRangeRow=r+j-1;
					break;
				}
			}
			//Finished checking
			int comboLength = rightRangeColumn - leftRangeColumn +1;
			if(combos.find(comboLength) != combos.end()){
				combos[comboLength] = combos[comboLength] +1; //Increase number of that combo length
			}else{
				combos[comboLength] =1; //one has been found of this length
			}
		}
	}//End loop of pieces
	return combos;
}


//Counts the number and combo size, of diagonal combos on the board
//Diagonal from x,0 to 0,x  like

map<int,int> gameBoard::checkDiagonalSE(int activePl){

	int n= getNumToWin(); //N - goal size
	int width= getWidth(); //Width of game board
	int height= getHeight(); //Height of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 =  playerPieces;
		p2 =  opponentPieces;
	}else{
		p1 =  opponentPieces;
		p2 =  playerPieces;
	}

	map<int, int> combos; //Stores combo numbers

	//Loop through active player's pieces
	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;


		//Check left of the piece
		int leftRangeColumn = c-(n-1);
		if(c-(n-1) < 0){ //Left column edge found
			leftRangeColumn = 0;
		}
		int leftRangeRow = r+(n-1);
		if(r+(n-1) > height-1){ //Left row edge found
			leftRangeRow = height-1;
		}

		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){

			if(p2[j].row <= leftRangeRow && p2[j].row > r){ //above piece
				if((p2[j].col >= leftRangeColumn) && (p2[j].col < c)){ //left of piece
					//In the range of blocking

					int val = leftRangeRow-r; //Only check in the non blocked range

					//count up to the determined val looking for blocking pieces
					for(int q=1; q<=val; q++){
						if(p2[j].row == r+q && p2[j].col == r-q){
							leftRangeColumn = p2[j].col+1; //potential range now lowered
							leftRangeRow = p2[j].row-1;

							val = leftRangeRow-r; //Update the value

						}
					}
				}
			}
		}
		//End of left check

		//CHECK RIGHT
		//Check right of the piece
		int rightRangeColumn = c+(n-1);
		if(c+(n-1) > width-1){ //right column edge found
			rightRangeColumn = width-1;
		}
		int rightRangeRow = r-(n-1);
		if(r-(n-1) < 0){ //right row edge found
			rightRangeRow = 0;
		}

		//Check for blocking opponent pieces
		for(unsigned j=0; j<p2.size(); j++){

			if(p2[j].row >= rightRangeRow && p2[j].row < r){
				if((p2[j].col <= rightRangeColumn) && (p2[j].col > c)){ //enemy piece in area of blocking
					//In the range of blocking

					int val = rightRangeColumn-c; //determine how many spaces to search

					//count up to the determined val looking for
					for(int q=1; q<=val; q++){
						if(p2[j].row == r-q && p2[j].col == r+q){
							rightRangeColumn = p2[j].col-1;
							rightRangeRow = p2[j].row+1;

							//reduce the range again
							val = rightRangeColumn-c;

						}
					}
				}
			}
		}
		//End of right check

		//See if combos are possible
		//If column OR row are n or smaller, then there is not enough space.
		if(rightRangeColumn - leftRangeColumn +1 < n){ //Range is not big enough to score a win.
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length rather than potential length

			//checking left
			for(int j=1;j<=c-leftRangeColumn;j++){
				bool found=false;
				for(unsigned k=0; k<p1.size();k++){ //Cycle through all friendly pieces
					if(p1[k].row==r+j){
						if(p1[k].col == c-j){ //Is this friendly piece in the right spot?
							found=true;
						}
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					leftRangeColumn=c-j+1;
					leftRangeRow=r+j-1;
					break;
				}
			}

			//checking right
			for(int j=1;j<=rightRangeColumn-c; j++){
				bool found=false;
				for(unsigned k=0; k<p1.size(); k++){
					if(p1[k].col == c+j && p1[k].row == r-j){ //Is this friendly piece in the right spot?
						found=true;
					}
				}
				if(!found){ //No friendly piece was found, combo broken.
					rightRangeColumn=c+j-1;
					rightRangeRow=r-j+1;
					break;
				}
			}
			//Finished checking
			int comboLength = rightRangeColumn - leftRangeColumn +1;
			if(combos.find(comboLength) != combos.end()){
				combos[comboLength] = combos[comboLength] +1; //Increase number of that combo length
			}else{
				combos[comboLength] =1; //one has been found of this length
			}
		}
	}//End loop of pieces
	return combos;
}
