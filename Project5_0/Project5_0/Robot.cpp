#include "Robot.h"
#include <algorithm> 
#include <math.h>
#include <queue>
#include <stack>

const int SCREEN_HEIGHT = 600;

vector<Vector2> finalbottomLines;
vector<Vector2> finaltopLines;

vector<Vector2> topVertices;
vector<Vector2> bottomVertices;
vector<Vector2> sideVertices;

struct Node {
	Vector2 point;
	Node* previous;
	int distance;
};


struct Point {
	int x;
	int y;
};


struct less_than_key {
	inline bool operator() (Node*& vec1, Node*& vec2)
	{
		return (vec1->distance < vec2->distance);
	}
};


struct greater_than_key {
	inline bool operator() (Block*& block1, Block*& block2)
	{
		return (block1->getHighY() > block2->getHighY());
	}
};


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

//Obvious function is obvious
bool samePoint(Vector2 p1, Vector2 p2) {
	if (p1.getX() == p1.getX() && p1.getY() == p2.getY())
		return true;
	return false;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r) {
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns the following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r) {
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

//Checks if a vertex is located inside another Square (Ignore that vertex if so)
bool vertexCollision(Vector2 p, vector<Vector2> top, vector<Vector2> bottom) {
	for (int x = 0; x < top.size(); x += 2) {

		if (top[x].getX() < top[x + 1].getX()) {
			if (!samePoint(p, top[x]) && p.getX() >= top[x].getX() && p.getX() <= top[x + 1].getX())
				if (!samePoint(p, bottom[x]) && p.getY() >= top[x].getY() && p.getY() <= bottom[x].getY())
					return true;
		}
		else {
			if (!samePoint(p, top[x]) && !samePoint(p, top[x + 1]) && p.getX() <= top[x].getX() && p.getX() >= top[x + 1].getX())
				if (!samePoint(p, bottom[x]) && !samePoint(p, bottom[x + 1]) && p.getY() >= top[x].getY() && p.getY() <= bottom[x].getY())
					return true;


		}
	}
	return false;
}

//Checks for line collision with squares
bool lineCollision(vector<Vector2> line) {
	Point p1;
	Point p2;
	p1.x = line[0].getX();
	p1.y = line[0].getY();
	p2.x = line[1].getX();
	p2.y = line[1].getY();
	bool result = false;

	//Checking the top of the squares
	for (int x = 0; x < bottomVertices.size(); x += 2) {
		Point p3;
		Point p4;
		p3.x = bottomVertices[x].getX();
		p3.y = bottomVertices[x].getY();
		p4.x = bottomVertices[x + 1].getX();
		p4.y = bottomVertices[x + 1].getY();
		if (doIntersect(p1, p2, p3, p4))
			result = true;
	}

	//Checking the bottom of the squares
	for (int x = 0; x < topVertices.size(); x += 2) {
		Point p3;
		Point p4;
		p3.x = topVertices[x].getX();
		p3.y = topVertices[x].getY();
		p4.x = topVertices[x + 1].getX();
		p4.y = topVertices[x + 1].getY();
		if (doIntersect(p1, p2, p3, p4))
			result = true;
	}

	//Checking the sides of the squares
	for (int x = 0; x < sideVertices.size(); x += 2) {
		Point p3;
		Point p4;
		p3.x = sideVertices[x].getX();
		p3.y = sideVertices[x].getY();
		p4.x = sideVertices[x + 1].getX();
		p4.y = sideVertices[x + 1].getY();
		if (doIntersect(p1, p2, p3, p4))
			result = true;
	}

	return result;
}

//Returns lines connecting from top of screen to top vertices
vector<Vector2> getTopLines(vector<Vector2> top, vector<Vector2> bottom, Vector2 p) {
	vector<Vector2> result;

	vector<Vector2> current_line;

	Vector2 v1;
	v1.setX(p.getX());
	v1.setY(0);

	Vector2 v2;
	v2.setX(p.getX());
	v2.setY(0);

	current_line.push_back(v1);
	current_line.push_back(v2);

	//Increment Y from top of screen to point of vertex, stop and split line if collision occurs
	while (current_line[1].getY() != p.getY()) {
		for (int x = 0; x < top.size(); x += 2) {
			vector<Vector2> currentTop;
			vector<Vector2> currentBottom;
			currentBottom.push_back(bottom[x]);
			currentBottom.push_back(bottom[x + 1]);
			currentTop.push_back(top[x]);
			currentTop.push_back(top[x + 1]);
			if (vertexCollision(current_line[1], currentTop, currentBottom)) {
				result.push_back(current_line[0]);
				result.push_back(current_line[1]);

				current_line[0].setY(currentBottom[0].getY());
				current_line[1].setY(currentBottom[0].getY());
			}
		}
		current_line[1].setY(current_line[1].getY() + 1);
	}

	result.push_back(current_line[0]);
	result.push_back(current_line[1]);

	return result;
}

vector<Vector2> getBottomLines(vector<Vector2> top, vector<Vector2> bottom, Vector2 p) {
	vector<Vector2> result;

	vector<Vector2> current_line;

	Vector2 v1;
	v1.setX(p.getX());
	v1.setY(SCREEN_HEIGHT);

	Vector2 v2;
	v2.setX(p.getX());
	v2.setY(SCREEN_HEIGHT);

	current_line.push_back(v1);
	current_line.push_back(v2);

	//Decrement Y from bottom of screen to point of vertex, stop and split line if collision occurs
	while (current_line[1].getY() != p.getY()) {
		for (int x = 0; x < top.size(); x += 2) {
			vector<Vector2> currentTop;
			vector<Vector2> currentBottom;
			currentBottom.push_back(bottom[x]);
			currentBottom.push_back(bottom[x + 1]);
			currentTop.push_back(top[x]);
			currentTop.push_back(top[x + 1]);
			if (vertexCollision(current_line[1], currentTop, currentBottom)) {
				result.push_back(current_line[0]);
				result.push_back(current_line[1]);

				current_line[0].setY(currentTop[0].getY());
				current_line[1].setY(currentTop[0].getY());
			}
		}
		current_line[1].setY(current_line[1].getY() - 1);
	}

	result.push_back(current_line[0]);
	result.push_back(current_line[1]);

	return result;
}

//Returns all possible neighbors to a node (meaning they don't intersect a square)
vector<Vector2> getNeighbors(Node* n, vector<Node*> allNodes) {
	Vector2 vec;
	vec.setX(n->point.getX());
	vec.setY(n->point.getY());

	vector<Vector2> result;

	for (int x = 0; x < allNodes.size(); ++x) {
		Vector2 vec1;
		vec1.setX(allNodes[x]->point.getX());
		vec1.setY(allNodes[x]->point.getY());
		vector<Vector2> new_line;
		new_line.push_back(vec);
		new_line.push_back(vec1);
		if (!lineCollision(new_line)) {
			Vector2 v;
			v.setX(allNodes[x]->point.getX());
			v.setY(allNodes[x]->point.getY());
			result.push_back(v);
		}
	}


	return result;
}

//Distance between two points
int distancePoint(Vector2 v1, Vector2 v2) {
	return sqrt(pow((v1.getX() - v2.getX()), 2) + pow(v1.getY() - v2.getY(), 2));
}

//Dijkstras shortest path algorithm
vector<Vector2> dijkstras(vector<Vector2> allPoints, Block* startPoint) {

	vector<Node*> allNodes;

	//Goal for checking later
	Vector2 goal;
	goal.setX(allPoints[allPoints.size() - 1].getX());
	goal.setY(allPoints[allPoints.size() - 1].getY());

	//Create initial set of all possible points
	for (int x = 0; x < allPoints.size(); ++x) {
		Node* temp = new Node();
		temp->previous = NULL;
		temp->distance = 999999999; //Basically Infinity ;)
		temp->point.setX(allPoints[x].getX());
		temp->point.setY(allPoints[x].getY());
		allNodes.push_back(temp);
	}

	//Now push in the start node with distance of 0, so itll be the first one out
	Node* start = new Node();
	start->previous = NULL;
	start->distance = 0;
	start->point.setX(startPoint->getCenter().getX());
	start->point.setY(startPoint->getCenter().getY());
	allNodes.push_back(start);

	while (!allNodes.empty()) {
		//Sort the nodes from smallest distance to greatest
		sort(allNodes.begin(), allNodes.end(), less_than_key());

		//Get smallest distance node
		Node* current = allNodes[0];

		//See if we are at the goal state so we can exit
		if (current->point.getX() == goal.getX() && current->point.getY() == goal.getY()) {
			break;
		}

		//If nah, remove the node we are about to relax on
		allNodes.erase(allNodes.begin());

		//Getting all possible neighbors (no box intersection points) still unrelaxed
		vector<Vector2> neighbors = getNeighbors(current, allNodes);

		//Relaxation of the graph for each neighbor
		for (int x = 0; x < neighbors.size(); ++x) {
			int expand;
			for (int y = 0; y < allNodes.size(); ++y)
				if (allNodes[y]->point.getX() == neighbors[x].getX() && allNodes[y]->point.getY() == neighbors[x].getY())
					expand = y;
			int test_distance = current->distance + distancePoint(current->point, allNodes[expand]->point);
			if (test_distance < allNodes[expand]->distance) {
				allNodes[expand]->distance = test_distance;
				allNodes[expand]->previous = current;
			}
		}
	}

	//After breaking, the first node in the vector will be the goal.
	//Creating stack through reverse interation of nodes, producing the best path
	stack<Node*> s;
	if (allNodes.size() > 0) {
		Node* current = allNodes[0];
		while (current->previous != NULL) {
			s.push(current);
			current = current->previous;
		}
	}

	vector<Vector2> result;

	//Starting point will be the first point in the result
	Vector2 start_point;
	start_point.setX(startPoint->getCenter().getX());
	start_point.setY(startPoint->getCenter().getY());
	result.push_back(start_point);

	//Push back the points in the correct order, FIFO stack
	while (!s.empty()) {
		Node * current = s.top();
		s.pop();
		Vector2 temp;
		temp.setX(current->point.getX());
		temp.setY(current->point.getY());
		result.push_back(temp);
	}

	return result;
}

vector<Vector2> Robot::findPath() {

	//Clear out our global variables (ALL great programs have global variables)
	topVertices.clear();
	bottomVertices.clear();
	sideVertices.clear();
	finalbottomLines.clear();
	finaltopLines.clear();

	//Used later for finding the left/rightmost points
	vector<float> smallestY(6, 900);
	vector<float> largestY(6, -1);

	//Sorting our squares from top to bottom
	sort(environment.begin(), environment.end(), greater_than_key());

	//Find left and rightmost vertices
	for (int y = 0; y < environment.size(); ++y)
		for (int x = 0; x < environment[y]->getVertices().size(); ++x) {
			if (largestY[y] < environment[y]->getVertices()[x].getY())
				largestY[y] = environment[y]->getVertices()[x].getY();
			if (smallestY[y] > environment[y]->getVertices()[x].getY())
				smallestY[y] = environment[y]->getVertices()[x].getY();
		}

	//creating the top vertices
	for (int y = 0; y < environment.size(); ++y)
		for (int x = 0; x < environment[y]->getVertices().size(); ++x)
			if (smallestY[y] == environment[y]->getVertices()[x].getY())
				topVertices.push_back(environment[y]->getVertices()[x]);

	//creating the bottom vertices
	for (int y = 0; y < environment.size(); ++y)
		for (int x = 0; x < environment[y]->getVertices().size(); ++x)
			if (largestY[y] == environment[y]->getVertices()[x].getY())
				bottomVertices.push_back(environment[y]->getVertices()[x]);

	//Get lines from the top vertices (vertical from top of screen to the point)
	for (int x = 0; x < topVertices.size(); ++x) {
		vector<Vector2> temp;
		if (!vertexCollision(topVertices[x], topVertices, bottomVertices)) {
			temp = getTopLines(topVertices, bottomVertices, topVertices[x]);
			for (int y = 0; y < temp.size(); ++y)
				finaltopLines.push_back(temp[y]);
		}
	}

	//Get lines from the bottom vertices (vertical from bottom of screen to the point)
	for (int x = 0; x < bottomVertices.size(); ++x) {
		vector<Vector2> temp;
		if (!vertexCollision(bottomVertices[x], topVertices, bottomVertices)) {
			temp = getBottomLines(topVertices, bottomVertices, bottomVertices[x]);
			for (int y = 0; y < temp.size(); ++y)
				finalbottomLines.push_back(temp[y]);
		}
	}

	//Create midpoints in lines from top and bottom vertices
	vector<Vector2> allPoints;
	for (int x = 0; x < finalbottomLines.size(); x += 2) {
		Vector2 temp;
		temp.setX(finalbottomLines[x].getX());
		temp.setY(abs(finalbottomLines[x].getY() - finalbottomLines[x + 1].getY()) / 2 + finalbottomLines[x + 1].getY());
		allPoints.push_back(temp);
	}

	for (int x = 0; x < finaltopLines.size(); x += 2) {
		Vector2 temp;
		temp.setX(finaltopLines[x].getX());
		temp.setY(abs(finaltopLines[x].getY() - finaltopLines[x + 1].getY()) / 2 + finaltopLines[x].getY());
		allPoints.push_back(temp);
	}

	//Get our side vertices for intersection checking in Dijkstra's
	for (int x = 0; x < environment.size(); ++x) {

		Vector2 vec1;
		vec1.setX(environment[x]->getLowX());
		vec1.setY(environment[x]->getLowY());

		Vector2 vec2;
		vec2.setX(environment[x]->getLowX());
		vec2.setY(environment[x]->getHighY());

		Vector2 vec3;
		vec3.setX(environment[x]->getHighX());
		vec3.setY(environment[x]->getLowY());

		Vector2 vec4;
		vec4.setX(environment[x]->getHighX());
		vec4.setY(environment[x]->getHighY());

		sideVertices.push_back(vec1);
		sideVertices.push_back(vec2);
		sideVertices.push_back(vec3);
		sideVertices.push_back(vec4);

	}

	
	vector<Vector2> path;

	//First, attempt to make a direct path from start to goal
	vector<Vector2> direct_path;
	Vector2 start;
	start.setX(startPoint->getCenter().getX());
	start.setY(startPoint->getCenter().getY());
	
	Vector2 end;
	end.setX(endPoint->getCenter().getX());
	end.setY(endPoint->getCenter().getY());

	direct_path.push_back(start);
	direct_path.push_back(end);

	//Simply return that path if successful
	if (!lineCollision(direct_path))
		return direct_path;

	//Otherwise, run Dijkstra's
	else {

		allPoints.push_back(endPoint->getCenter());
		path = dijkstras(allPoints, startPoint);

		return path;
	}
}
