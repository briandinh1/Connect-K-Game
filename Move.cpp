#include "Move.h"

Move::Move() : col(0), row(0) {}

Move::Move(int col, int row)
{
	this->row=row;
	this->col=col;
}

void Move::setCoordinates(int col, int row)
{
	this->col = col;
	this->row = row;
}

void Move::setCoordinates(const Move& move)
{
	this->col = move.col;
	this->row = move.row;
}