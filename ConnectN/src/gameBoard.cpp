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

	//change the storage of pieces
	for(int i=0; i< playerPieces.size(); i++) {
		if(playerPieces[i].col == col){
			playerPieces[i].row = playerPieces[i].row + 1;
			if(playerPieces[i].row > height-1){


				playerPieces.erase(playerPieces.begin() +i);
			}
		}
	}

	for(int i=0; i< opponentPieces.size(); i++) {
		if(opponentPieces[i].col == col){
			opponentPieces[i].row = opponentPieces[i].row + 1;
			if(opponentPieces[i].row > height-1){


				opponentPieces.erase(opponentPieces.begin() +i);
			}
		}
	}

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
	else if(!globals.isPopOutLeft)
	{
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

//	cerr <<"Lo  ||";
//	for(map<int, int>::const_iterator it = Lo.begin();
//			it != Lo.end(); ++it)
//	{
//		if(it->first != 1 && it->first != 0){
//			cerr << "-" <<it->first<<"- "<<it->second << "\t";
//		}
//	}
//	cerr << "\nLp  ||";
//	for(map<int, int>::const_iterator it = Lp.begin();
//			it != Lp.end(); ++it)
//	{
//		if(it->first != 1 && it->first != 0){
//			cerr << "-" <<it->first<<"- "<<it->second << "\t";
//		}
//	}
//	cerr << "\n";

	if(Lo[n] >= 1 && Lp[n] >= 1){
		//cerr << "Tie\n";
		return 100; //If both opponent and player have multiple wins, it would be a tie. This AI decides ties are better than normal moves, but worse than wins
	}else if(Lo[n] >= 1){
		//cerr<< "Lo wins\n";
		if(isMyTurn){
			return -1;
		}else{
			return 100;
		} //If opponent has a line combo of n or more, that is a loss. Worst choice
	}else if(Lp[n] >= 1){
		//cerr << "Lp Wins\n";
		if(isMyTurn){
			return 150;
		}else{
			return -1;
		} //If player has line combo of n or more, that is a win. Best choice!
	}

	double oval=0.1;

	for(map<int, int>::const_iterator it = Lo.begin();
			it != Lo.end(); ++it)
	{
		if(it->first != 1 && it->first != 0){
			oval += it->second * it->first * it->first;
		}
	}

	double pval=0.1;

	for(map<int, int>::const_iterator it = Lp.begin();
			it != Lp.end(); ++it)
	{
		if(it->first != 1 && it->first != 0){
			pval += it->second * it->first * it->first;
		}
	}

	if(isMyTurn){
		hVal = pval / oval;
	}else{
		hVal = oval/pval;
	}
	//hVal = pval / oval;

	cerr << "pval/oval="<<pval<<"/"<<oval<<"="<<hVal<<"\n";

	//Real return
	return hVal;
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
	char cell, ocell;
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 = playerPieces;
		p2 = opponentPieces;
		cell = globals.ownCell;
		ocell = globals.opponentCell;
	}else{
		p1 = opponentPieces;
		p2 = playerPieces;
		cell = globals.opponentCell;
		ocell = globals.ownCell;
	}

	map<int, int> combos; //Stores combo numbers
	map<int, bool> used;
	//Loop through active player's pieces
	for(unsigned piece=0; piece<p1.size(); piece++){
		int r = p1[piece].row;
		int c = p1[piece].col;


		//-------CHECKING LEFT-------//
		//Check left of the piece n-1 spaces (to see if a n sized combo possible)
		int leftRange = c-(n-1);
		if(c-(n-1) < 0){ //Left edge found
			leftRange = 0; //Reduce range
		}
		int ldistance = c-leftRange;

		//Check for blocking opponent pieces
		for(int j=1; j<=ldistance; j++){
			if(board[r][c-j]== ocell){
				leftRange = p1[piece].col-j+1;
				break;
			}
		}
		//--------END LEFT CHECK--------//

		//-------CHECKING RIGHT-------//
		//Check left of the piece n-1 spaces (to see if a n sized combo possible)
		int rightRange = c+(n-1);
		if(c+(n-1) > width-1){ //Left edge found
			rightRange = width-1; //Reduce range
		}
		int rdistance = rightRange-c;

		//Check for blocking opponent pieces
		for(int j=1; j<=rdistance; j++){
			if(board[r][c+j]== ocell){
				rightRange = p1[piece].col+j-1;
				break;
			}
		}
		//--------END RIGHT CHECK--------//

		//See if combos are possible
		if(rightRange - leftRange +1 < n){ //Range is not big enough to score a win
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			//cerr <<"Origin piece:" << r <<","<<c<<"\tRanges"<<leftRange<<","<<rightRange<<"\n";
			if(used[piece] == true) break;
			used[piece]=true;
			//p1.erase(p1.begin() +piece);//Removes self so it doesnt use this to search

			//comboLeft and comboRight used to determine combo length like ranges
			int comboLeftRange=c, comboRightRange=c;
			//checking left
			//------CHECKING LEFT COMBO------//
			for(int k=c-1;k>=leftRange-1; k--){
				//cerr << "   checking left k="<<k<<"\t"<<board[r][k]<<"\n";
				if(board[r][k] == cell){
					comboLeftRange = k;

					for(unsigned j=0; j<p1.size(); j++){
						if(p1[j].row == r && p1[j].col == k){
							used[j]=true;
							//p1.erase(p1.begin() +j); //Remove so its not double counted
						}
					}
				}else{
					break;
				}
			}
			//------END LEFT COMBO CHECK-----//

			//------CHECKING RIGHT COMBO------//

			for(int k=c+1;k<=rightRange+1; k++){
				//cerr << "   checking right k="<<k<<"\t"<<board[r][k]<<"\n";
				if(board[r][k] == cell){
					//cerr << "  found friendly right at " << k <<"\n";
					comboRightRange = k;

					for(unsigned j=0; j<p1.size(); j++){
						if(p1[j].row == r && p1[j].col == k){
							used[j]=true;
							//p1.erase(p1.begin() +j); //Remove so its not double counted
						}
					}
				}else{
					break;
				}
			}
			//------END RIGHT COMBO CHECK-----//



			int comboLength = abs(comboRightRange - comboLeftRange) +1;
			//cerr << "CRR"<<comboRightRange<<" CLR"<<comboLeftRange<<" = "<< comboLength<<"\n";
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
	char cell, ocell;
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	if(activePl == PLAYER){
		p1 = playerPieces;
		p2 = opponentPieces;
		cell = globals.ownCell;
		ocell = globals.opponentCell;
	}else{
		p1 = opponentPieces;
		p2 = playerPieces;
		cell = globals.opponentCell;
		ocell = globals.ownCell;
	}

	map<int, int> combos; //Stores combo numbers
	map<int,bool> used;
	//cerr << "CHECK VERT\n";
	//Loop through active player's pieces
	for(unsigned piece=0; piece<p1.size(); piece++){

		int r = p1[piece].row;
		int c = p1[piece].col;
		//cerr << "Viewing piece ["<<r<<","<<c<<"]\n";

		//-------CHECKING DOWN-------//
		//Check left of the piece n-1 spaces (to see if a n sized combo possible)
		int bottomRange = r+(n-1);
		if(r+(n-1) > height-1){ //bottom edge found
			bottomRange = height-1; //Reduce range
		}
		int bdistance = bottomRange -r;

		//Check for blocking opponent pieces
		for(int j=1; j<=bdistance; j++){
			//cerr << "  chk blk ["<<r+j<<","<<c<<"]\n";
			if(board[r+j][c]== ocell){
				bottomRange = p1[piece].row+j-1;
				break;
			}
		}
		//--------END DOWN CHECK--------//

		//-------CHECKING ABOVE-------//
		//Check left of the piece n-1 spaces (to see if a n sized combo possible)
		int topRange = r-(n-1);
		if(r-(n-1) < 0){ //Left edge found
			topRange = 0; //Reduce range
		}
		int tdistance = r-topRange;

		//Check for blocking opponent pieces
		for(int j=1; j<=tdistance; j++){
			//cerr << "  chk blk ["<<r-j<<","<<c<<"]\n";
			if(board[r-j][c]== ocell){
				topRange = p1[piece].row-j+1;
				break;
			}
		}
		//--------END ABOVE CHECK--------//

		//See if combos are possible
		if(bottomRange - topRange +1 < n){ //Range is not big enough to score a win
			//Too bad, don't count the combo
			//cerr << "Range too small\n";
		}else{ //Possible combo, look for actual combo existing
			//cerr <<"Origin piece:" << r <<","<<c<<"\tRanges"<<bottomRange<<","<<topRange<<"\n";
			if(used[piece] == true) break;
			used[piece]=true;
			//p1.erase(p1.begin() +piece);//Removes self so it doesnt use this to search

			//comboLeft and comboRight used to determine combo length like ranges
			int comboTopRange=r, comboBottomRange=r;
			//checking left
			//------CHECKING BELOW COMBO------//
			for(int k=r+1;k<=bottomRange+1; k++){
				if(k > height-1) break;
				//cerr << "   checking below k="<<k<<"\t"<<board[k][c]<<"\n";
				if(board[k][c] == cell){
					//cerr << "  found friendly below at " << k <<"\n";
					comboBottomRange = k;

					for(unsigned j=0; j<p1.size(); j++){
						if(p1[j].row == k && p1[j].col == c){
							used[j]=true;
							//p1.erase(p1.begin() +j); //Remove so its not double counted
						}
					}
				}else{
					break;
				}
			}
			//------END BELOW COMBO CHECK-----//

			//------CHECKING ABOVE COMBO------//

			for(int k=r-1;k>=topRange-1; k--){
				//cerr << "   checking above k="<<k<<"\t"<<board[k][c]<<"\n";
				if(board[k][c] == cell){
					//cerr << "  found friendly above at " << k <<"\n";
					comboTopRange = k;

					for(unsigned j=0; j<p1.size(); j++){
						if(p1[j].row == k && p1[j].col == c){
							used[j]=true;
							//p1.erase(p1.begin() +j); //Remove so its not double counted
						}
					}
				}else{
					break;
				}
			}
			//------END RIGHT COMBO CHECK-----//



			int comboLength = abs(comboBottomRange - comboTopRange)+1;
			//cerr << "CTR"<<comboTopRange<<" CBR"<<comboBottomRange<<" = "<< comboLength<<"\n";
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
map<int,int> gameBoard::checkDiagonalNE(int activePl){

	int n=  getNumToWin(); //N - goal size
	int width= getWidth(); //Width of game board
	int height= getHeight(); //Height of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	char cell, ocell;

	if(activePl == PLAYER){
		p1 = playerPieces;
		p2 = opponentPieces;
		cell = globals.ownCell;
		ocell = globals.opponentCell;
	}else{
		p1 = opponentPieces;
		p2 = playerPieces;
		cell = globals.opponentCell;
		ocell = globals.ownCell;
	}

	map<int, int> combos; //Stores combo numbers
	map<int, bool> used;
	//Loop through active player's pieces
	//cerr << "CHECK DIANE\n";
	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;
		//cerr << "Viewing piece ["<<r<<","<<c<<"]\n";

		//Check left of the piece
		int leftRangeColumn = c-(n-1);
		if(c-(n-1) < 0){ //Left column edge found
			leftRangeColumn = 0;
		}
		int leftRangeRow = r-(n-1);
		if(r+(n-1) > height-1){ //Left row edge found
			leftRangeRow = height-1;
		}

		//Check for blocking opponent pieces

		int lval = leftRangeRow -r; //How many spaces to search
		for(int q=1; q <=lval; q++){
			if(board[r+q][c-q] == ocell){
				leftRangeColumn = c-q+1;
				leftRangeRow = r+q-1;

				lval = leftRangeRow-r;
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

		//check for blocks
		int rval = r-rightRangeRow;
		for(int q=1; q<=rval; q++){
			if(board[r-q][c+q] == ocell){
				leftRangeColumn = c+q-1;
				leftRangeRow = r-q+1;

				rval = r-rightRangeRow;
			}
		}
		//End of right check

		//See if combos are possible
		//If column OR row are n or smaller, then there is not enough space.
		//cerr << "CHCK CMBOS\n";
		if(rightRangeColumn - leftRangeColumn +1 < n){ //Range is not big enough to score a win.
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			//cerr <<"Origin piece:" << r <<","<<c<<"\tRanges"<<leftRangeRow<<","<<rightRangeRow<<"\n";
			if(used[i] == true) break;
			used[i] = true;
			//p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length rather than potential length

			int comboLeftRange=r, comboRightRange=r;
			//checking left
			for(int j=1; j<=c-leftRangeColumn; j++){
				//cerr << "chk lft\n";
				if(r+j > height-1 || c-j < 0) break;
				//cerr << "   checking left ["<<r+j<<","<< c-j<<"\t"<<board[r+j][c-j]<<"\n";
				if(board[r+j][c-j] == cell){
					//cerr << "  Friendly left at [" << r-j <<","<< c+j<<"]\n";
					comboLeftRange = r+j;

					for(unsigned k=0; k<p1.size(); k++){
						if(p1[k].row == r+j && p1[k].col == c-j){
							//cerr << "    Removing [" << r-j <<","<< c+j<<"]\n";
							used[k]=true;
							//p1.erase(p1.begin() +k); //Remove so its not double counted
						}
					}
				}else{
					//cerr << "  CLR="<< comboLeftRange<<"\n";
					break;
				}
			}

			//checking right
			for(int j=1; j<=rightRangeColumn-c; j++){
				//cerr <<"chk rigt\n";
				if(c+j > width-1 || r-j < 0) break;
				//cerr << "   Checking right ["<<r-j<<","<< c+j<<"\t"<<board[r-j][c+j]<<"\n";
				if(board[r-j][c+j] == cell){
					//cerr << "  Friendly right at [" << r-j <<","<< c+j<<"]\n";
					comboRightRange = r-j;

					for(unsigned k=0; k<p1.size(); k++){
						if(p1[k].row == r-j && p1[k].col == c+j){
							//cerr << "    Removing [" << r-j <<","<< c+j<<"]\n";
							used[k] = true;
							//p1.erase(p1.begin() +k); //Remove so its not double counted
						}
					}
				}else{
					//cerr << "  CLR="<< comboRightRange<<"\n";
					break;
				}
			}
			//Finished checking

			int comboLength = abs(comboRightRange - comboLeftRange) +1;
			//cerr << "CRR"<<comboRightRange<<" CLR"<<comboLeftRange<<" = "<< comboLength<<"\n";
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
map<int,int> gameBoard::checkDiagonalSE(int activePl){

	int n=  getNumToWin(); //N - goal size
	int width= getWidth(); //Width of game board
	int height= getHeight(); //Height of game board
	vector<GamePiece> p1;
	vector<GamePiece> p2;

	char cell, ocell;

	if(activePl == PLAYER){
		p1 = playerPieces;
		p2 = opponentPieces;
		cell = globals.ownCell;
		ocell = globals.opponentCell;
	}else{
		p1 = opponentPieces;
		p2 = playerPieces;
		cell = globals.opponentCell;
		ocell = globals.ownCell;
	}

	map<int, int> combos; //Stores combo numbers

	//Loop through active player's pieces
	//cerr << "CHECK DiaSE\n";
	for(vector<GamePiece>::const_iterator it = opponentPieces.begin();
			it != opponentPieces.end(); ++it)
	{
		GamePiece i = *it;
		//cerr << "  OPiece[" << i.row << "," << i.col << "]\n";
	}

	map<int,bool> used;

	for(unsigned i=0; i<p1.size(); i++){
		int r = p1[i].row;
		int c = p1[i].col;
		//	cerr << "Viewing piece ["<<r<<","<<c<<"]\n";

		//Check left of the piece
		int leftRangeColumn = c-(n-1);
		if(c-(n-1) < 0){ //Left column edge found
			leftRangeColumn = 0;
		}
		int leftRangeRow = r-(n-1);
		if(r-(n-1) < 0){ //Left row edge found
			leftRangeRow = 0;
		}

		//Check for blocking opponent pieces
		//cerr << "chk left block\n";
		int lval = r-leftRangeRow; //How many spaces to search
		for(int q=1; q <=lval; q++){
			if(r+q < 0 || c-q < 0) break;
			if(board[r-q][c-q] == ocell){
				leftRangeColumn = c-q+1;
				leftRangeRow = r-q+1;

				lval = r-leftRangeRow;
			}
		}
		//End of left check

		//CHECK RIGHT
		//cerr << "chk right block\n";
		//Check right of the piece
		int rightRangeColumn = c+(n-1);
		if(c+(n-1) > width-1){ //right column edge found
			rightRangeColumn = width-1;
		}
		int rightRangeRow = r+(n-1);
		if(r+(n-1) > height-1){ //right row edge found
			rightRangeRow = height-1;
		}

		//check for blocks
		int rval = rightRangeRow-r;
		for(int q=1; q<=rval; q++){
			if(r+q > height-1 || c+q > height-1) break;
			if(board[r+q][c+q] == ocell){
				leftRangeColumn = c+q-1;
				leftRangeRow = r+q-1;

				rval = rightRangeRow-r;
			}
		}
		//End of right check

		//See if combos are possible
		//If column OR row are n or smaller, then there is not enough space.
		//cerr << "CHCK CMBOS\n";
		if(rightRangeColumn - leftRangeColumn +1 < n){ //Range is not big enough to score a win.
			//Too bad, don't count the combo
		}else{ //Possible combo, look for actual combo existing
			//cerr <<"Origin piece:" << r <<","<<c<<"\tRanges"<<leftRangeRow<<","<<rightRangeRow<<"\n";

			//cerr << "before used\n";
			if(used[i] == true) break; //Already used in a horizontal calculation, ignore.
			//cerr << "past used\n";
			used[i]=true;
			//p1.erase(p1.begin() +i);//Removes self so it doesnt use this to search

			//leftRange and rightRange is now used to determine combo length rather than potential length

			int comboLeftRange=r, comboRightRange=r;
			//checking left
			for(int j=1; j<=c-leftRangeColumn; j++){
				//cerr << "chk lft\n";
				if(r-j < 0 || c-j < 0) break;
				//cerr << "   checking left ["<<r-j<<","<< c-j<<"\t"<<board[r-j][c-j]<<"\n";
				if(board[r-j][c-j] == cell){
					//cerr << "  Friendly left at [" << r-j <<","<< c-j<<"]\n";
					comboLeftRange = r-j;

					for(unsigned k=0; k<p1.size(); k++){
						if(p1[k].row == r-j && p1[k].col == c-j){
							//cerr << "    Removing [" << r-j <<","<< c-j<<"]\n";
							used[k]=true;
							//p1.erase(p1.begin() +k); //Remove so its not double counted
						}
					}
				}else{
					//cerr << "  CLR="<< comboLeftRange<<"\n";
					break;
				}
			}

			//checking right
			for(int j=1; j<=rightRangeColumn-c; j++){
				//cerr <<"chk rigt\n";
				if(c+j > width-1 || r+j > height-1) break;
				//cerr << "   Checking right ["<<r+j<<","<< c+j<<"\t"<<board[r+j][c+j]<<"\n";
				if(board[r+j][c+j] == cell){
					//cerr << "  Friendly right at [" << r+j <<","<< c+j<<"]\n";
					comboRightRange = r+j;

					for(unsigned k=0; k<p1.size(); k++){
						if(p1[k].row == r+j && p1[k].col == c+j){
							//cerr << "    Removing [" << r+j <<","<< c+j<<"]\n";
							used[k]=true;
							//p1.erase(p1.begin() +k); //Remove so its not double counted
						}
					}
				}else{
					//cerr << "  CLR="<< comboRightRange<<"\n";
					break;
				}
			}
			//Finished checking

			int comboLength = abs(comboRightRange - comboLeftRange) +1;
			//cerr << "CRR"<<comboRightRange<<" CLR"<<comboLeftRange<<" = "<< comboLength<<"\n";
			if(combos.find(comboLength) != combos.end()){
				combos[comboLength] = combos[comboLength] +1; //Increase number of that combo length
			}else{
				combos[comboLength] =1; //one has been found of this length
			}
		}
	}//End loop of pieces
	return combos;
}
