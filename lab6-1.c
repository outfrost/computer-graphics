#include "essential_3d.c"

void draw_triangle() {
	flat_color(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-4.5f, -3.0f, 1.0f);
	glVertex3f(0.0f, 4.6f, -1.0f);
	glVertex3f(4.5f, -3.0f, 1.0f);
	glEnd();
}

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	look_at_origin();
	draw_axes();
	adjust_modelview_rotation();
	
	if (viewmode & 1<<2)
		draw_triangle();
	if (viewmode & 1<<1)
		
	if (viewmode & 1<<0)
		
	
	glFlush();
	
	glutSwapBuffers();
}

void init_render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	GLfloat light0_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_position[] = {0.0f, 0.0f, 10.0f, 1.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
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
	glutMouseFunc(mouse_button_event);
	glutMotionFunc(mouse_motion_event);
	
	init_render();
	
	glutMainLoop();
	return 0;
}
