#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <stdio.h>
#include <windows.h> 
#include <ctime>
#include <cstdlib>
#include <vector>

#include <GL/glut.h>

#include "Block.h"
#include "Robot.h"
#include "Vector2.h"

using namespace std;

enum SetupState { Ready, DBlock1, Block1, DBlock2, Block2, DBlock3, Block3, StartPoint, EndPoint, Restart, BadState};

class GUI {
private:
	static GUI* s_instance;
	GUI();
	GUI(int SCREEN_WIDTH, int SCREEN_HEIGHT, float ambient_light);

	SetupState state;

	// Holds the three blocks that the path searches around
	vector<Block*> environment;

	// The start point for the path
	Block* startPoint;

	// The end point for the path
	Block* endPoint;

	// The list of coordinates for the path (a line is drawn between these points)
	vector<Vector2> robotPath;

	// What color the screen is cleared for (1 = white, 0 = black)
	float ambient_light = 0.7f;

	// The size of the start/end points (the width of the boxes)
	const int pointsize = 5;
	const int pathsize = 3;
	int SCREEN_WIDTH, SCREEN_HEIGHT;
public:
	static GUI* getInstance();
	static GUI* getInstance(int SCREEN_WIDTH, int SCREEN_HEIGHT, float ambient_light);


	// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
	void drawEnvironment();		// Draws the squares
	void drawStartPoint();		// Draws the start point
	void drawEndPoint();		// Draws the end point
	void drawPath();			// Draws the path determined by the Robot class

	void processClick(int x, int y);	// The x/y coordinates of a mouse click (only when the mouse is pressed down)
	void processMotion(int x, int y);	// The x/y coordinates when the mouse is moved (to drag-create boxes)

	void initMessage();		// The message that is displayed when the GUI is ready
	// =~=~=~
	static void display();	// Draws everything
	static void mouseAction(int button, int state, int x, int y);	// OnClick listener for the window
	static void mouseMovement(int x, int y);	// OnMove listener for the window
};

#endif