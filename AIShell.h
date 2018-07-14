#ifndef AISHELL_H
#define AISHELL_H

#pragma once
#include "Move.h"
#include <climits>
#include <ctime>

// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type.

	// PLAYER1 representing max, PLAYER2 representing min
	static const int PLAYER1 = 1;
	static const int PLAYER2 = -1;
	static const int NO_PIECE = 0;

	int deadline; 
	int k;   //how many connected pieces needed to win

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
	~AIShell();
	Move makeMove();


private:
	int numRows;
	int numCols;
	int **gameState;
	bool gravityOn;
	Move lastMove; //this is the move made by opponent. if player moves first then it == (-1, -1)

	int piecesLeft;
	bool checkedPBM; // so that the next iterations of maxPlayer() won't keep looking at previousBestMove
	Move previousBestMove; 
	time_t startTime;

	Move alphaBeta();

	// gravity off 
	int maxPlayer(Move& bestMove, int depth,
		int alpha = INT_MIN, int beta = INT_MAX);
	int minPlayer(Move& bestMove, int depth,
		int alpha = INT_MIN, int beta = INT_MAX);

	// gravity on
	int maxPlayerG(Move& bestMove, int depth,
		int alpha = INT_MIN, int beta = INT_MAX);
	int minPlayerG(Move& bestMove, int depth,
		int alpha = INT_MIN, int beta = INT_MAX);

	int evaluation();
	void countFrame(int col, int row, int x, int y, int& score);

	Move checkValidMove(const Move& m);
	Move backupMove();

	void checkDeadline();

};

#endif //AISHELL_H