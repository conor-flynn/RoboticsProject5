#ifndef ROBOT_H
#define ROBOT_H

#include "Block.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class Robot {
private:

	// Dimensions of the screen (Never changes)
	int SCREEN_WIDTH, SCREEN_HEIGHT;

	// The three squares that you have to path-find around
	vector<Block*> environment;

	// The start point
	Block* startPoint;

	// The end point
	Block* endPoint;
public:

	Robot(int SCREEN_WIDTH, int SCREEN_HEIGHT, vector<Block*> environment, Block* startPoint, Block* endPoint);

	// =~=~=~=~=~=~=~=~=~=~

	// Returns a vector of Vector2's which are just (x,y) coordinates for the path to be drawn
	vector<Vector2> findPath();

};

#endif