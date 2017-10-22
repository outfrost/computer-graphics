#include <GL/gl.h>
#include <GL/glut.h>

void render_scene() {
	// Clear the stage using the current clear colour
	glClear(GL_COLOR_BUFFER_BIT);
	// Flush draw calls to execution
	glFlush();
}

void init_render() {
	// Set the clear colour to grey
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	// Set display mode: single frame buffer, RGBA colour model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	// Create a window (stage) with a title
	glutCreateWindow("First OpenGL application");
	
	// Set rendering callback to render_scene()
	// GLUT will call this function whenever a stage redraw is necessary
	glutDisplayFunc(render_scene);
	
	init_render();
	
	// Start the GLUT framework main loop
	glutMainLoop();
}
