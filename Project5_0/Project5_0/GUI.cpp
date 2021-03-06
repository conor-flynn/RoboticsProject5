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

	float xx, yy, width, height;
	
	switch (state) {
	case Ready:

		width = height = block1Size;
		xx = x - width / 2;
		yy = y - height / 2;
		environment.push_back(new Block(xx, yy, width, height));
		state = Block1;
		cout << "Click somewhere to place the second block(" << block2Size << "x" << block2Size << ")." << endl;
		break;

	case Block1:

		width = height = block2Size;
		xx = x - width / 2;
		yy = y - height / 2;
		environment.push_back(new Block(xx, yy, width, height));
		state = Block2;
		cout << "Click somewhere to place the third block(" << block3Size << "x" << block3Size << ")." << endl;
		break;

	case Block2:

		width = height = block3Size;
		xx = x - width / 2;
		yy = y - height / 2;
		environment.push_back(new Block(xx, yy, width, height));
		state = Block3;
		cout << "Click somewhere to place the starting point." << endl;
		break;

	case Block3:

		width = height = pointsize;
		xx = x - width / 2;
		yy = y - height / 2;
		startPoint = new Block(xx, yy, width, height);
		state = StartPoint;
		cout << "Click somewhere to place the ending point." << endl;
		break;

	case StartPoint:

		width = height = pointsize;
		xx = x - width / 2;
		yy = y - height / 2;
		endPoint = new Block(xx, yy, width, height);
		state = EndPoint;
		cout << "Click somewhere to launch the path-finder." << endl;
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

	defaut:
		break;
	}
}

void GUI::initMessage() {
	cout << "Click somewhere to place the first block(" << block1Size << "x" << block1Size << ")." << endl;
}


void GUI::display() {
	glClearColor(GUI::getInstance()->ambient_light, GUI::getInstance()->ambient_light, GUI::getInstance()->ambient_light, 0);
	glClear(GL_COLOR_BUFFER_BIT);

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
