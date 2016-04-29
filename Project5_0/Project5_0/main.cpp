
#include "GUI.h"

#include <GL/glut.h>

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

const int WINDOW_START_X = 800;
const int WINDOW_START_Y = 300;

const float ambient_light = 0.3f;



int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(WINDOW_START_X, WINDOW_START_Y);
	glutCreateWindow("Robotis - Project 5");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);

	glClearColor(ambient_light, ambient_light, ambient_light, 0);

	cout << "Setup complete.\n\n\n\n\n\n" << endl;

	GUI::getInstance(SCREEN_WIDTH, SCREEN_HEIGHT, ambient_light);

	glutDisplayFunc(GUI::display);
	glutMouseFunc(GUI::mouseAction);
	glutPassiveMotionFunc(GUI::mouseMovement);
	glutMainLoop();	

	return 0;
}

