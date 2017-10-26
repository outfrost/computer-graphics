#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <time.h>

//typedef float point2f[2];
typedef struct {
	float x;
	float y;
} point2f;

float randf() {
	return (float)rand() / (float)RAND_MAX;
}

bool* carpet(int iterations) {
	int size = 1;
	for (int i = iterations; i > 0; --i)
		size *= 3;
	
	bool* points = malloc(size * size * sizeof(bool));
	for (int i = 0; i < size * size; ++i)
		*(points+i) = true;
	
	for (int square_size = size; iterations > 0; --iterations) {
		square_size /= 3;
		for (int x = size - 1; x >= 0; --x) {
			int y = size - 1;
			for (int y = size - 1; y >= 0; --y) {
				if ((x / square_size) % 3 == 1 && (y / square_size) % 3 == 1)
					points[x * size + y] = false;
			}
		}
	}
}

void draw_square(point2f point, float size) {
	float halfsize = size / 2;
	float deviation = size / 4;
	
	glBegin(GL_POLYGON);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x - halfsize + randf() * deviation, point.y - halfsize + randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x + halfsize + randf() * deviation, point.y - halfsize + randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x + halfsize + randf() * deviation, point.y + halfsize + randf() * deviation);
	glColor3f(randf(), randf(), randf());
	glVertex2f(point.x - halfsize + randf() * deviation, point.y + halfsize + randf() * deviation);
	glEnd();
}

void render_scene() {
	// Clear the stage using the current clear colour
	glClear(GL_COLOR_BUFFER_BIT);
	
	int iterations = 1
	bool * points = carpet(iterations);
	
	for (int x = 0; x <  ; ++x)
	
	// Flush draw calls to execution
	glFlush();
}

void init_render() {
	// Set the clear colour to grey
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
		glOrtho(-100.0, 100.0, -100.0/aspect_ratio, 100.0/aspect_ratio, 1.0, -1.0);
	else
		glOrtho(-100.0*aspect_ratio, 100.0*aspect_ratio, -100.0, 100.0, 1.0, -1.0);
	
	// Set the coordinate system and bounding space again
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	srand(time(NULL));
	
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
