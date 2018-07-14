#ifndef MOVE_H
#define MOVE_H

#pragma once
class Move
{
public:
	int row; 
	int col; 
	Move();
	Move(int col, int row);

// -----------------------------------------------------------------------------

	void setCoordinates(int col, int row);
	void setCoordinates(const Move& move);
};

#endif //MOVE_H
