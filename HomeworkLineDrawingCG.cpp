#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include<stdio.h>

/* Salih Emre Torunoğlu 190201041
This code uses the DDA or Bresenham algorithm based on the selection through the menu
to draw a line or a circle between the first two points in the points vector. 
The center of the circle is the first point in the points vector and the second point is 
used as the radius of the circle. When the user right-clicks on the point after selecting it with the left-click, the selected point is removed from the vector. 
We can also select the algorithm and shape to be drawn through the menu with middle button of the mouse.
*/
using namespace std;

vector<pair<int, int> > points;
int selected = -1;
int algorithm = 1; 
int shape = 1; 
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
}

void DDA(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = max(abs(dx), abs(dy));
	float x_increment = (float)dx / (float)steps;
	float y_increment = (float)dy / (float)steps;
	float x = x1;
	float y = y1;
	glBegin(GL_POINTS);
	for (int i = 0; i <= steps; i++) {
		glVertex2i((int)x, (int)y);
		x += x_increment;
		y += y_increment;
	}
	glEnd();
}

void Bresenham(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int x_sign = 1;
	int y_sign = 1;
	if (dx < 0) {
		x_sign = -1;
		dx = -dx;
	}
	if (dy < 0) {
		y_sign = -1;
		dy = -dy;
	}
	int p = 2 * dy - dx;
	int x = x1;
	int y = y1;
	glBegin(GL_POINTS);
	for (int i = 0; i <= dx; i++) {
		glVertex2i(x, y);
		if (p >= 0) {
			x += x_sign;
			y += y_sign;
			p += 2 * (dy - dx);
		} else {
			x += x_sign;
			p += 2 * dy;
		}
	}
	glEnd();
}

void MidPointCircle(int x_center, int y_center, int radius) {
	int x = 0;
	int y = radius;
	int p = 1 - radius;
	glBegin(GL_POINTS);
	while (x <= y) {
		glVertex2i(x_center + x, y_center + y);
		glVertex2i(x_center + y, y_center + x);
		glVertex2i(
			x_center - x, y_center + y);
		glVertex2i(x_center - y, y_center + x);
		glVertex2i(x_center - x, y_center - y);
		glVertex2i(x_center - y, y_center - x);
		glVertex2i(x_center + x, y_center - y);
		glVertex2i(x_center + y, y_center - x);
		if (p < 0) {
			p += 2 * x + 3;
		} else {
			p += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(-250, 0);
	glVertex2i(250, 0);
	glVertex2i(0, -250);
	glVertex2i(0, 250);
	glEnd();
	for (int i = 0; i < points.size(); i++) {
		if (i == selected) {
			glColor3f(1.0, 0.0, 0.0);
		} else {
			glColor3f(0.0, 0.0, 1.0);
		}
		glBegin(GL_POINTS);
		glVertex2i(points[i].first, points[i].second);
		glEnd();
	}
	if (points.size() > 1 && shape == 1) {
		if(algorithm == 1)
			DDA(points[0].first, points[0].second, points[1].first, points[1].second);
		else
			Bresenham(points[0].first, points[0].second, points[1].first, points[1].second);
	}
	if (points.size() > 1 && shape == 2) {
		int radius = (int)sqrt(pow(points[1].first - points[0].first, 2) + pow(points[1].second - points[0].second, 2));
		MidPointCircle(points[0].first, points[0].second, radius);
	}
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int x_coord = x - 250;
		int y_coord = 250 - y;
		for (int i = 0; i < points.size(); i++) {
			int x_diff = x_coord - points[i].first;
			int y_diff = y_coord - points[i].second;
			if (x_diff * x_diff + y_diff * y_diff <= 25) {
				selected = i;
				glutPostRedisplay();
				return;
			}
		}
		points.push_back(make_pair(x_coord, y_coord));
		glutPostRedisplay();
	} else if (
		button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (selected != -1) {
			points.erase(points.begin() + selected);
			selected = -1;
			glutPostRedisplay();
		}
	}
}

void menu(int value) {
	switch (value) {
	case 1:
		algorithm = 1;
		break;
	case 2:
		algorithm = 2;
		break;
	case 3:
		shape = 1;
		break;
	case 4:
		shape = 2;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	int chs;
	printf("Name and student number: Salih Emre Torunoglu-190201041\n");
	printf("\n------------------------------------------------------------------\n");
	printf("Steps to run this computer graphics homework\n");
	printf("\n------------------------------------------------------------------\n");
	printf("Step1: Type Any Key and Hit Enter\n");
	printf("\n******************************************************************\n\n");
	scanf("%d",&chs);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2D Coordinate System with DDA/Bresenham Line and Circle");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutCreateMenu(menu);
	glutAddMenuEntry("DDA Algorithm", 1);
	glutAddMenuEntry("Bresenham Algorithm", 2);
	glutAddMenuEntry("Line", 3);
	glutAddMenuEntry("Circle", 4);
	glutAttachMenu(GLUT_MIDDLE_BUTTON); //For the menu
	glutMainLoop();
	return 0;
}


