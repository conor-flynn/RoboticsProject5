#include "Block.h"




Block::Block(int startx, int starty, int width, int height) {
	this->startx = startx;
	this->starty = starty;
	this->width = width;
	this->height = height;

	
	c11 = random();
	c12 = random();
	c13 = random();
	c21 = random();
	c22 = random();
	c23 = random();
	c31 = random();
	c32 = random();
	c33 = random();
	c41 = random();
	c42 = random();
	c43 = random();
	
}

void Block::resize(int x, int y) {
	this->width	 = x - this->startx;
	this->height = y - this->starty;
}

void Block::draw() {

	/*
	// Don't like the rainbow color nonsense, use this:
	glBegin(GL_QUADS);
	glVertex2f(startx, starty);
	glVertex2f(startx, starty + height);
	glVertex2f(startx + width, starty + height);
	glVertex2f(startx + width, starty);
	glEnd();	
	
	*/

	glBegin(GL_QUADS);

	glColor3f(c11, c12, c13);
	glVertex2f(startx, starty);

	glColor3f(c21, c22, c23);
	glVertex2f(startx, starty + height);

	glColor3f(c31, c32, c33);
	glVertex2f(startx + width, starty + height);

	glColor3f(c41, c42, c43);
	glVertex2f(startx + width, starty);

	glEnd();
	

	
}

Vector2 Block::getCenter() {
	return Vector2(startx + (width / 2), starty + (height / 2));
}

int Block::getLowX() {

	return (width < 0) ? (startx + width) : (startx);
}
int Block::getHighX() {
	return (width < 0) ? (startx) : (startx + width);
}

int Block::getLowY() {
	return (height < 0) ? (starty + height) : (starty);
}

int Block::getHighY() {
	return (height < 0) ? (starty) : (starty + height);
}

float Block::random() {
	return ((float)rand() / RAND_MAX);
}

vector<Vector2> Block::getVertices() {
	vector<Vector2> result;

	result.push_back(Vector2(startx, starty));
	result.push_back(Vector2(startx, starty+height));
	result.push_back(Vector2(startx+width, starty+height));
	result.push_back(Vector2(startx+width, starty));

	return result;
}
