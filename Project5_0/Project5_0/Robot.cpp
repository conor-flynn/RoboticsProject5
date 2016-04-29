#include "Robot.h"




Robot::Robot(int SCREEN_WIDTH, int SCREEN_HEIGHT, vector<Block*> environment, Block* startPoint, Block* endPoint) {
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
	for (Block* block : environment) {
		this->environment.push_back(block);
	}
	this->startPoint = startPoint;
	this->endPoint = endPoint;
}

/*
	(0,0) is the top left corner
	(SCREEN_WIDTH, SCREEN_HEIGHT) is the bottom right corner

	block.getLowX()  : returns the x-coordinate of the block that is closest to (0,0)
	block.getHighX() : returns the x-coordinate of the block that is closest to (WIDTH, 0)
	block.getLowY()  : does the same, but with the y-coordinate

	block.getCenter() : Vector2 that is the center of the block
*/
vector<Vector2> Robot::findPath() {

	/*
	
		Path finding stuff goes here 8^)
	
	*/



	vector<Vector2> path;

	path.push_back(startPoint->getCenter());
	path.push_back(environment[0]->getCenter());
	path.push_back(environment[1]->getCenter());
	path.push_back(environment[2]->getCenter());
	path.push_back(endPoint->getCenter());

	return path;
}
