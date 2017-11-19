#include <GL/glut.h>
//#include <math.h>
#include <string.h>
#include <stdio.h>

typedef float point3f[3];

const float AXIS_RADIUS = 5.0f;
const float MIN_FOV_ORTHO = 10.0f;

short viewmode = 0x1;
GLfloat camera[] = {0.0f, 0.0f, 10.0f};

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

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	gluLookAt(camera[0], camera[1], camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	draw_axes();
	
/*	if (viewmode & 1<<2)
		draw_egg_faces();
	if (viewmode & 1<<1)
		draw_egg_edges();
	if (viewmode & 1<<0)
		draw_egg_vertices();
*/	
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(3.0);
	
	glFlush();
	
	glutSwapBuffers();
}

void init_render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize_stage(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(70.0, (GLdouble)width/(GLdouble)height, 1.0, 30.0);
	
//	GLfloat aspect_ratio = (GLfloat)width / (GLfloat)height;
	
/*	if (width <= height)
		glOrtho(-MIN_FOV_ORTHO, MIN_FOV_ORTHO, -MIN_FOV_ORTHO/aspect_ratio, MIN_FOV_ORTHO/aspect_ratio, 128.0, -128.0);
	else
		glOrtho(-MIN_FOV_ORTHO*aspect_ratio, MIN_FOV_ORTHO*aspect_ratio, -MIN_FOV_ORTHO, MIN_FOV_ORTHO, 128.0, -128.0);
*/	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void key_pressed(unsigned char keycode, int x, int y) {
	if (keycode == 'p')
		viewmode ^= 1<<0;
	else if (keycode == 'w')
		viewmode ^= 1<<1;
	else if (keycode == 's')
		viewmode ^= 1<<2;
	
	render_scene();
}

char * get_gl_info() {
	const char * gl_version = glGetString(GL_VERSION);
	const char * gl_sl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char * gl_renderer = glGetString(GL_RENDERER);
	
	size_t gl_info_length = strlen("OpenGL  - GLSL  - ")
				+ strlen(gl_version)
				+ strlen(gl_sl_version)
				+ strlen(gl_renderer);
	
	char * gl_info = malloc(gl_info_length + sizeof(char));
	sprintf(gl_info, "OpenGL %s - GLSL %s - %s", gl_version, gl_sl_version, gl_renderer);
	
	return gl_info;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitWindowSize(640, 480);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	glutSetWindowTitle(get_gl_info());
	
	glutDisplayFunc(render_scene);
	glutReshapeFunc(resize_stage);
	glutKeyboardFunc(key_pressed);
	
	init_render();
	
	glEnable(GL_DEPTH_TEST);
	
	glutMainLoop();
	return 0;
}
