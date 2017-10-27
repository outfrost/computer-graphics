#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

typedef float point3f[3];

const float AXIS_RADIUS = 5.0f;
const unsigned int EGG_SUBDIVISIONS = 10u;

point3f * egg_vertices = NULL;

point3f * get_vertex(int i, int k) {
	return &(egg_vertices[i * EGG_SUBDIVISIONS + k]);
}

void create_vertex(float u, float v, point3f * result) {
	(*result)[0] = (-  90.0f * powf(u, 5.0f)
			+ 225.0f * powf(u, 4.0f)
			- 270.0f * powf(u, 3.0f)
			+ 180.0f * powf(u, 2.0f)
			-  45.0f * u) * cosf (M_PI * v);
	
	(*result)[1] =    160.0f * powf(u, 4.0f)
			- 320.0f * powf(u, 3.0f)
			+ 160.0f * powf(u, 2.0f);
	
	(*result)[2] = (-  90.0f * powf(u, 5.0f)
			+ 225.0f * powf(u, 4.0f)
			- 270.0f * powf(u, 3.0f)
			+ 180.0f * powf(u, 2.0f)
			-  45.0f * u) * sinf(M_PI * v);
}

void compute_egg_vertices() {
	egg_vertices = realloc(egg_vertices, EGG_SUBDIVISIONS * EGG_SUBDIVISIONS * sizeof(point3f));
	
	for (int i = 0; i < EGG_SUBDIVISIONS; ++i) {
		float u = (1.0f / EGG_SUBDIVISIONS) * i;
		for (int k = 0; k < EGG_SUBDIVISIONS; ++k) {
			float v = (1.0f / EGG_SUBDIVISIONS) * k;
			create_vertex(u, v, get_vertex(i, k));
		}
	}
}

void draw_axes() {
	point3f x_axis_start = { -AXIS_RADIUS, 0.0f, 0.0f };
	point3f x_axis_end = { AXIS_RADIUS, 0.0f, 0.0f };
	point3f y_axis_start = { 0.0f, -AXIS_RADIUS, 0.0f };
	point3f y_axis_end = { 0.0f, AXIS_RADIUS, 0.0f };
	point3f z_axis_start = { 0.0f, 0.0f, -AXIS_RADIUS };
	point3f z_axis_end = { 0.0f, 0.0f, AXIS_RADIUS };
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3fv(x_axis_start);
		glVertex3fv(x_axis_end);
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3fv(y_axis_start);
		glVertex3fv(y_axis_end);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3fv(z_axis_start);
		glVertex3fv(z_axis_end);
	glEnd();
}

void draw_egg() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
		for (int i = 0; i < EGG_SUBDIVISIONS; ++i) {
			for (int k = 0; k < EGG_SUBDIVISIONS; ++k) {
				point3f * vertex = get_vertex(i, k);
				glVertex3f((*vertex)[0], (*vertex)[1], (*vertex)[2]);
			}
		}
	glEnd();
}

void render_scene() {
	// Clear the stage using the current clear colour
	// Note the additional GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Reset the current transform matrix (?)
	glLoadIdentity();
	
	draw_axes();
	draw_egg();
	
	// Flush draw calls to execution
	glFlush();
	
	glutSwapBuffers();
}

void init_render() {
	// Set the clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize_stage(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	// Set device viewport dimensions
	glViewport(0, 0, width, height);
	// Set the coordinate system to reference the observer
	glMatrixMode(GL_PROJECTION);
	// Reset the current transform matrix (?)
	glLoadIdentity();
	
	GLfloat aspect_ratio = (GLfloat)width / (GLfloat)height;
	
	if (width <= height)
		glOrtho(-17.5, 17.5, -17.5/aspect_ratio, 17.5/aspect_ratio, 10.0, -10.0);
	else
		glOrtho(-17.5*aspect_ratio, 17.5*aspect_ratio, -17.5, 17.5, 10.0, -10.0);
	
	// Set the coordinate system and bounding space again
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	compute_egg_vertices();
	
	glutInit(&argc, argv);
	
	glutInitWindowSize(400, 300);
	
	// Set display mode: single frame buffer, RGBA colour model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	// Create a window (stage) with a title
	glutCreateWindow("3D coordinate system");
	
	// Set rendering callback to render_scene()
	// GLUT will call this function whenever a stage redraw is necessary
	glutDisplayFunc(render_scene);
	
	// Set the stage reshape handling function to resize_stage()
	glutReshapeFunc(resize_stage);
	
	init_render();
	
	// Enable the removal of invisible surfaces
	glEnable(GL_DEPTH_TEST);
	
	// Start the GLUT framework main loop
	glutMainLoop();
}
