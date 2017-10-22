#include <GL/gl.h>
#include <GL/glut.h>

void render_scene() {
	// Clear the stage using the current clear colour
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Set the drawing colour to green
	glColor3f(0.0f, 1.0f, 0.0f);
	// Draw a triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 50.0f);
	glVertex2f(50.0f, 0.0f);
	glEnd();
	
	// Set the drawing colour to red
	glColor3f(1.0f, 0.0f, 0.0f);
	// Draw a triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 50.0f);
	glVertex2f(-50.0f, 0.0f);
	glEnd();
	
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
	
	// Set display mode: single frame buffer, RGBA colour model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	// Create a window (stage) with a title
	glutCreateWindow("Third OpenGL application");
	
	// Set rendering callback to render_scene()
	// GLUT will call this function whenever a stage redraw is necessary
	glutDisplayFunc(render_scene);
	
	// Set the stage reshape handling function to resize_stage()
	glutReshapeFunc(resize_stage);
	
	init_render();
	
	// Start the GLUT framework main loop
	glutMainLoop();
}
