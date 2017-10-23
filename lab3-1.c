#include <GL/gl.h>
#include <GL/glut.h>

typedef float point3f[3];

const float AXIS_RADIUS = 5.0f;

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

void draw_teapot() {
	glRotated(60.0, 1.0, 1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(3.0);
	glLoadIdentity();
}

void render_scene() {
	// Clear the stage using the current clear colour
	// Note the additional GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Reset the current transform matrix (?)
	glLoadIdentity();
	
	draw_axes();
	draw_teapot();
	
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
		glOrtho(-7.5, 7.5, -7.5/aspect_ratio, 7.5/aspect_ratio, 10.0, -10.0);
	else
		glOrtho(-7.5*aspect_ratio, 7.5*aspect_ratio, -7.5, 7.5, 10.0, -10.0);
	
	// Set the coordinate system and bounding space again
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
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
