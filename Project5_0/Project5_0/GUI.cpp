#include "GUI.h"


GUI* GUI::s_instance;
GUI* GUI::getInstance() {
	if (s_instance == NULL) {
		cout << "No window dimensions given. This is really bad :(" << endl;
		s_instance = new GUI(0,0,0);
	}
	return s_instance;
}

GUI * GUI::getInstance(int SCREEN_WIDTH, int SCREEN_HEIGHT, float ambient_light) {
	if (s_instance == NULL) {
		s_instance = new GUI(SCREEN_WIDTH, SCREEN_HEIGHT, ambient_light);
	}
	return s_instance;
}


GUI::GUI() {
	state = SetupState::BadState;
	this->SCREEN_WIDTH = 0;
	this->SCREEN_HEIGHT = 0;

	cout << "Bad init" << endl;
}

GUI::GUI(int SCREEN_WIDTH, int SCREEN_HEIGHT, float ambient_light) {
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
	this->ambient_light = ambient_light;

	state = SetupState::Ready;

	startPoint = NULL;
	endPoint = NULL;

	initMessage();
}

void GUI::drawEnvironment() {
	glColor3f(0.7f, 0.7f, 0.7f);
	for (Block* block : environment) {
		block->draw();
	}
}
void GUI::drawStartPoint() {
	glColor3f(0, 1, 0);
	if (startPoint != NULL) startPoint->draw();
}
void GUI::drawEndPoint() {
	glColor3f(1, 0, 0);
	if (endPoint != NULL) endPoint->draw();
}
void GUI::drawPath() {
	glColor3f(0, 0, 1);
	glLineWidth(pathsize);
	glBegin(GL_LINE_STRIP);
	for (Vector2 v : robotPath) {
		glVertex2f(v.getX(), v.getY());
	}
	glEnd();
}

void GUI::processClick(int x, int y) {
	// enum SetupState { Nothing, DBlock1, Block1, DBlock2, Block2, DBlock3, Block3, StartPoint, EndPoint };
	switch (state) {
	case Ready:

		// We have nothing yet, so create the first block
		environment.push_back(new Block(x, y, 0, 0));
		state = DBlock1;
		cout << "Move the mouse around to resize, and click when you are finished." << endl << endl;

		break;
	case DBlock1:

		// We are finished drawing the first block
		environment[0]->resize(x, y);
		state = Block1;
		cout << "Click where you want the second Block to start from. Move the mouse around to resize, and click when you are finished." << endl << endl;

		break;
	case Block1:

		// We need to start drawing the next block
		environment.push_back(new Block(x, y, 0, 0));
		state = DBlock2;

		break;
	case DBlock2:

		// We have finished drawing block2
		environment[1]->resize(x, y);
		state = Block2;
		cout << "Click where you want the third Block to start from. Move the mouse around to resize, and click when you are finished." << endl << endl;

		break;
	case Block2:

		// We need to start drawing the next block
		environment.push_back(new Block(x, y, 0, 0));
		state = DBlock3;

		break;
	case DBlock3:

		// We have finished drawing block3
		environment[2]->resize(x, y);
		state = Block3;
		cout << "Click where you want the starting location to be." << endl << endl;

		break;
	case Block3:

		// We need to save the location and construct the block that represents the start point
		startPoint = new Block(x - pointsize, y - pointsize, 2 * pointsize, 2 * pointsize);
		state = StartPoint;
		cout << "Click where you want the ending location to be." << endl << endl;

		break;
	case StartPoint:

		// We need to save the location and construct the block that represents the end point
		endPoint = new Block(x - pointsize, y - pointsize, 2 * pointsize, 2 * pointsize);
		state = EndPoint;
		cout << "Click when you want to find the path between the start and end locations." << endl << endl;

		break;
	case EndPoint:
	{
		// Ask for the path from the robot
		Robot* robot = new Robot(SCREEN_WIDTH, SCREEN_HEIGHT, environment, startPoint, endPoint);
		vector<Vector2> path = robot->findPath();
		for (Vector2 v : path) {
			robotPath.push_back(Vector2(v));
		}
		state = Restart;
		cout << "Click again to clear and restart." << endl << endl;

	}break;
	case Restart:
		cout << "Attempting to restart..." << endl << endl;

		environment.clear();
		delete(startPoint);
		delete(endPoint);
		robotPath.clear();

		state = Ready;
		initMessage();

		break;
	default:
		break;
	}
}

void GUI::processMotion(int x, int y) {
	switch (state) {
	case Ready:
		break;
	case DBlock1:

		// Update the width and height of block 1
		environment[0]->resize(x, y);

		break;
	case Block1:
		break;
	case DBlock2:

		// Update the width and height of block 2
		environment[1]->resize(x, y);

		break;
	case Block2:
		break;
	case DBlock3:

		// Update the width and height of block 3
		environment[2]->resize(x, y);
		break;
	case Block3:
		break;
	case StartPoint:
		break;
	case EndPoint:
		break;
	default:
		break;
	}
}

void GUI::initMessage() {
	cout << "Click somewhere to determine the starting location of the first Block" << endl << endl;
}


void GUI::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(GUI::getInstance()->ambient_light, GUI::getInstance()->ambient_light, GUI::getInstance()->ambient_light,0);

	GUI::getInstance()->drawEnvironment();
	GUI::getInstance()->drawStartPoint();
	GUI::getInstance()->drawEndPoint();
	GUI::getInstance()->drawPath();

	glutSwapBuffers();
}

void GUI::mouseAction(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)	GUI::getInstance()->processClick(x, y);
	glutPostRedisplay();
}
void GUI::mouseMovement(int x, int y) {
	GUI::getInstance()->processMotion(x, y);
	glutPostRedisplay();
}
