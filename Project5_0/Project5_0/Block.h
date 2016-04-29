#ifndef BLOCK_H
#define BLOCK_H


#include "Vector2.h"
#include <vector>
#include <stdio.h>
#include <windows.h> 
#include <ctime>
#include <iostream>

#include "GL/glut.h"

using namespace std;

/*
	A structure to draw squares
*/

class Block {

	// Keep these private you naughty person!
	// The true center of the Block is none of these values
	int startx;		// Where the mouse was clicked
	int starty;		// Where the mouse was clicked
	int width;		// How far the mouse was dragged
	int height;		// How far the mouse was dragged

	// Colors for the vertices
	float c11, c12, c13;
	float c21, c22, c23;
	float c31, c32, c33;
	float c41, c42, c43;

public:
	Block(int startx, int starty, int width, int height);

	// Used when the Block is being constructed and the user is dragging the mouse around
	void resize(int x, int y);

	// Draws the Block to the screen
	void draw();

	// Returns the center of the Block
	Vector2 getCenter();

	// (0,0) is the top left corner
	// (WIDTH, HEIGHT) is the bottom right corner
	int getLowX();
	int getHighX();

	int getLowY();
	int getHighY();

	// Returns a random float between 0 and 1
	float random();

	vector<Vector2> getVertices();
};

#endif