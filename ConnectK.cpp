#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "AIShell.h"
#include "Move.h"


using namespace std;

bool isFirstPlayer = false;

AIShell* makeAIShellFromInput(){

	AIShell* shell = NULL;

	string begin =  "makeMoveWithState:";
	string end = "end";
	string input;
	bool go = true;
	while (go){
		cin >> input;
		if (input == end){
			exit(0);
		}
		else if (input == begin){

			// create game board attributes from input from GUI
			int g;
			cin >> g;
			bool gravity=true;
			if (g==0){
				gravity = false;
			}
			cin >> g;
			int colCount = g;
			cin >>g;
			int rowCount = g;
			cin >>g;
			int lastMoveCol = g;
			cin >> g;
			int lastMoveRow = g;


			int deadline = -1;
			cin >>g;
			deadline = g;

			cin >> g;
			int k = g;


			//allocate 2D array
			int **gameState = NULL;
			gameState = new int*[colCount];
			for (int i =0; i<colCount; i++){
				gameState[i] = new int[rowCount];
			}

			int countMoves = 0;
			for (int col =0; col<colCount; col++){
				for (int row =0; row<rowCount; row++){
					cin >> gameState[col][row];
					if (gameState[col][row] != AIShell::NO_PIECE)
					{
						countMoves += gameState[col][row];
					}
				}
			}

			if (countMoves % 2 == 0)
			{
				isFirstPlayer = true;
			}

			Move m(lastMoveCol, lastMoveRow);
			AIShell* shell = new AIShell (colCount, rowCount, gravity, gameState, m);
			shell->deadline=deadline;
			shell->k=k;

			return shell;
		}
		else {
			cout<<"unrecognized command "<< input<<endl;
		}
	}

	return shell;
}

void returnMove(Move move){
	string madeMove = "ReturningTheMoveMade";
	cout<<madeMove<<" "<<move.col<<" "<<move.row<<endl;
	cout.flush();
}


bool checkIfFirstPlayer()
{
	return isFirstPlayer;
}

int main() {
	bool go = true;
	while (go){ // do forever until GUI kills it
	AIShell *shell = makeAIShellFromInput();
	Move moveMade = shell->makeMove();
	returnMove(moveMade);
	delete shell;
	}

	return 0;
}