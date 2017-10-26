#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <time.h>

//typedef float point2f[2];
typedef struct {
	float x;
	float y;
} point2f;

const int iterations = 4;
bool * points = NULL;
int cells = 0;

float randf() {
	return (float)rand() / (float)RAND_MAX;
}

int powi(int base, int power) {
	int result = 1;
	for (; power > 0; --power)
		result *= base;
	return result;
}

void carpet(int iterations) {
	cells = powi(3, iterations);
	
	points = realloc(points, cells * cells * sizeof(bool));
	for (int i = 0; i < cells * cells; ++i)
		*(points+i) = true;
	
	for (int square_size = cells; iterations > 0; --iterations) {
		square_size /= 3;
		for (int x = cells - 1; x >= 0; --x) {
			int y = cells - 1;
			for (int y = cells - 1; y >= 0; --y) {
				if ((x / square_size) % 3 == 1 && (y / square_size) % 3 == 1)
					points[x * cells + y] = false;
			}
		}
	}
}

void draw_square(point2f point, float size) {
	float deviation = size / 4;
	
	glBegin(GL_POLYGON);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x 	  + randf() * deviation, point.y	+ randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x + size + randf() * deviation, point.y	+ randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x + size + randf() * deviation, point.y + size + randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x 	  + randf() * deviation, point.y + size + randf() * deviation);
	glEnd();
}

void render_scene() {
	// Clear the stage using the current clear colour
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (int x = 0; x < cells; ++x) {
		for (int y = 0; y < cells; ++y) {
			if (points[x * cells + y])
				draw_square((point2f) { (-50.0f + x * (100.0f / cells)), (-50.0f + y * (100.0f / cells)) }, 100.0f / cells);
		}
	}
	
	// Flush draw calls to execution
	glFlush();
}

void init_render() {
	// Set the clear colour to dark grey
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void resize_stage(GLsizei width, GLsizei height) {
	GLfloat aspect_ratio;
	
	if (height == 0)
		height = 1;
	
	// Set device viewport dimensions
	glViewport(0, 0, width, height);
	// Set the coordinate system to reference the observer
	glMatrixMode(GL_PROJECTION);
	// Set the bounding space (?)
	glLoadIdentity();
	
	aspect_ratio = (GLfloat)width / (GLfloat)height;
	
	if (width <= height)
		glOrtho(-65.0,65.0, -65.0/aspect_ratio, 65.0/aspect_ratio, 1.0, -1.0);
	else
		glOrtho(-65.0*aspect_ratio, 65.0*aspect_ratio, -65.0, 65.0, 1.0, -1.0);
	
	// Set the coordinate system and bounding space again
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	srand(time(NULL));
	
	carpet(iterations);
	
	// Set display mode: single frame buffer, RGBA colour model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	// Create a window (stage) with a title
	glutCreateWindow("Fifth OpenGL application");
	
	// Set rendering callback to render_scene()
	// GLUT will call this function whenever a stage redraw is necessary
	glutDisplayFunc(render_scene);
	
	// Set the stage reshape handling function to resize_stage()
	glutReshapeFunc(resize_stage);
	
	init_render();
	
	// Start the GLUT framework main loop
	glutMainLoop();
}
