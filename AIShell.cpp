#include "AIShell.h"
#include <iostream>
#include <climits>
#include <ctime>
#include <fstream>

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline = 0;
	this->numRows = numRows;
	this->numCols = numCols;
	this->gravityOn = gravityOn;
	this->gameState = gameState;
	this->lastMove = lastMove;

	piecesLeft = 0;

	for (int col = 0; col < numCols; ++col)
		for (int row = 0; row < numRows; ++row)
			if (gameState[col][row] == NO_PIECE) ++piecesLeft;
}

AIShell::~AIShell()
{
	//delete the gameState variable.
	for (int i = 0; i < numCols; ++i){
		delete[] gameState[i];
	}
	delete[] gameState;
}

Move AIShell::makeMove(){
	// if first player, no pieces on the board, place first piece in the center
	if (lastMove.col == -1 && lastMove.row == -1)
		return Move(numCols / 2, numRows / 2);

	return alphaBeta();
}

// ================================================================================================
// ================================================================================================

Move AIShell::alphaBeta()
{
	try
	{
		startTime = time(0);
		Move bestMove;
		int depth = 1;

		while (true)
		{
			if (piecesLeft == 1) return backupMove();
			if (piecesLeft - depth < 0) return checkValidMove(previousBestMove);
			checkedPBM = false;
			(gravityOn)
				? maxPlayerG(bestMove, depth++)
				: maxPlayer(bestMove, depth++);
			previousBestMove.setCoordinates(bestMove);
		}
	}
	catch (int e)
	{
		return checkValidMove(previousBestMove);
	}

	return backupMove();
}


int AIShell::maxPlayer(Move& bestMove, int depth, int alpha, int beta)
{
	if (depth == 0) return evaluation();

	checkDeadline();

	int value = alpha;
	Move bestCurrentMove;

	// look at previous best move first, because it has a higher chance of leading to better moves
	if (!checkedPBM && gameState[previousBestMove.col][previousBestMove.row] == NO_PIECE)
	{
		gameState[previousBestMove.col][previousBestMove.row] = PLAYER1;
		int temp = minPlayer(bestMove, depth - 1, value, beta);
		gameState[previousBestMove.col][previousBestMove.row] = NO_PIECE;
		checkedPBM = true;

		if (temp > value)
		{
			value = temp;
			bestCurrentMove.setCoordinates(previousBestMove.col, previousBestMove.row);
		}

		if (value > beta)
		{
			bestMove.setCoordinates(bestCurrentMove);
			return beta;
		}
	}

	// look at the rest of the possible moves
	for (int col = 0; col < numCols; ++col)
	{
		for (int row = 0; row < numRows; ++row)
		{
			if (col == previousBestMove.col && row == previousBestMove.row) continue;

			if (gameState[col][row] == NO_PIECE)
			{
				gameState[col][row] = PLAYER1;
				int temp = minPlayer(bestMove, depth - 1, value, beta);
				gameState[col][row] = NO_PIECE;

				if (temp > value)
				{
					value = temp;
					bestCurrentMove.setCoordinates(col, row);
				}

				if (value > beta)
				{
					bestMove.setCoordinates(bestCurrentMove);
					return beta;
				}
			}

			checkDeadline();
		}
	}

	bestMove.setCoordinates(bestCurrentMove);
	return value;
}


int AIShell::minPlayer(Move& bestMove, int depth, int alpha, int beta)
{
	if (depth == 0) return evaluation();

	checkDeadline();

	int value = beta;
	Move bestCurrentMove;

	for (int col = 0; col < numCols; ++col)
	{
		for (int row = 0; row < numRows; ++row)
		{
			if (gameState[col][row] == NO_PIECE)
			{
				gameState[col][row] = PLAYER2;
				int temp = maxPlayer(bestMove, depth - 1, alpha, value);
				gameState[col][row] = NO_PIECE;

				if (temp < value)
				{
					value = temp;
					bestCurrentMove.setCoordinates(col, row);
				}
				if (value < alpha)
				{
					bestMove.setCoordinates(bestCurrentMove);
					return alpha;
				}
			}

			checkDeadline();
		}
	}

	bestMove.setCoordinates(bestCurrentMove);
	return value;
}

// ------------------------------------------------------------------------------------------------

int AIShell::maxPlayerG(Move& bestMove, int depth, int alpha, int beta)
{
	if (depth == 0) return evaluation();

	checkDeadline();

	int value = alpha;
	Move bestCurrentMove;

	// look at previous best move first, because it has a higher chance of leading to better moves
	if (!checkedPBM && gameState[previousBestMove.col][previousBestMove.row] == NO_PIECE)
	{
		gameState[previousBestMove.col][previousBestMove.row] = PLAYER1;
		int temp = minPlayerG(bestMove, depth - 1, value, beta);
		gameState[previousBestMove.col][previousBestMove.row] = NO_PIECE;
		checkedPBM = true;

		if (temp > value)
		{
			value = temp;
			bestCurrentMove.setCoordinates(previousBestMove.col, previousBestMove.row);
		}

		if (value > beta)
		{
			bestMove.setCoordinates(bestCurrentMove);
			return beta;
		}
	}

	// look at the rest of the possible moves
	for (int col = 0; col < numCols; ++col)
	{
		if (col == previousBestMove.col) continue;

		if (gameState[col][numRows - 1] == NO_PIECE)
		{
			gameState[col][numRows - 1] = PLAYER1;
			int temp = minPlayer(bestMove, depth - 1, value, beta);
			gameState[col][numRows - 1] = NO_PIECE;

			if (temp > value)
			{
				value = temp;
				bestCurrentMove.setCoordinates(col, numRows - 1);
			}

			if (value > beta)
			{
				bestMove.setCoordinates(bestCurrentMove);
				return beta;
			}
		}

		checkDeadline();
	}

	bestMove.setCoordinates(bestCurrentMove);
	return value;
}


int AIShell::minPlayerG(Move& bestMove, int depth, int alpha, int beta)
{
	if (depth == 0) return evaluation();

	checkDeadline();

	int value = beta;
	Move bestCurrentMove;

	for (int col = 0; col < numCols; ++col)
	{
		if (gameState[col][numRows - 1] == NO_PIECE)
		{
			gameState[col][numRows - 1] = PLAYER2;
			int temp = maxPlayerG(bestMove, depth - 1, alpha, value);
			gameState[col][numRows - 1] = NO_PIECE;

			if (temp < value)
			{
				value = temp;
				bestCurrentMove.setCoordinates(col, numRows - 1);
			}
			if (value < alpha)
			{
				bestMove.setCoordinates(bestCurrentMove);
				return alpha;
			}
		}

		checkDeadline();
	}

	bestMove.setCoordinates(bestCurrentMove);
	return value;
}

// ================================================================================================
// ================================================================================================

int AIShell::evaluation()
{
	int score = 0;

	for (int col = 0; col <= numCols - k; ++col)
		for (int row = 0; row < numRows; ++row)
			countFrame(col, row, 1, 0, score); // horizontal

	for (int col = 0; col < numCols; ++col)
		for (int row = 0; row <= numRows - k; ++row)
			countFrame(col, row, 0, 1, score); // vertical

	for (int col = 0; col <= numCols - k; ++col)
		for (int row = numRows - k; row >= 0; --row)
			countFrame(col, row, 1, 1, score);// diagonal up

	for (int col = 0; col <= numCols - k; ++col)
		for (int row = k - 1; row < numRows; ++row)
			countFrame(col, row, 1, -1, score); // diagonal down

	return score;
}


void AIShell::countFrame(int col, int row, int x, int y, int& score)
{
	int p1 = 0;
	int p2 = 0;

	for (int i = 0; i < k; ++i)
	{
		if (gameState[col + i * x][row + i * y] == PLAYER1) ++p1;
		else if (gameState[col + i * x][row + i * y] == PLAYER2) ++p2;

		if (p1 > 1 && p2 > 1) return;
	}

	if (p1 == k) score += 10000;
	else if (p1 == k - 1 && p2 == 0) score += 5000;
	else if (p1 == k - 2 && p2 == 0) score += 2500;
	else score += p1 * p1;

	if (p2 == k) score -= 100000;
	else if (p2 == k - 1 && p1 == 0) score -= 50000;
	else if (p2 == k - 2 && p1 == 0) score -= 25000;
	else score -= p2 * p2;
}

// ================================================================================================
// ================================================================================================

Move AIShell::checkValidMove(const Move& m)
{
	return (gameState[m.col][m.row] == NO_PIECE) ? m : backupMove();
}


Move AIShell::backupMove()
{
	for (int col = 0; col < numCols; ++col)
		for (int row = 0; row < numRows; ++row)
			if (gameState[col][row] == NO_PIECE)
				return Move(col, row);
}


void AIShell::checkDeadline()
{
	if (time(0) - startTime >= deadline / 1000) throw - 1;
}