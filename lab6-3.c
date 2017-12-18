#include <stdio.h>
#include "essential_3d.c"
#include "tga.c"

void draw_tetrahedron() {
	flat_color(1.0f, 1.0f, 1.0f, 1.0f);
	
	glBegin(GL_TRIANGLES);
	
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(0.5f, 0.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -2.1666f, 4.3334f);
	
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-4.5f, 2.1666f, 2.1666f);
	glVertex3f(-4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(-4.5f, 2.1666f, 2.1666f);
	glVertex3f(0.0f, -2.1666f, 4.3334f);
	glTexCoord2f(0.5f, 0.0f);
	glNormal3f(-4.5f, 2.1666f, 2.1666f);
	glVertex3f(0.0f, 4.3334f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(4.5f, 2.1666f, 2.1666f);
	glVertex3f(0.0f, -2.1666f, 4.3334f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(4.5f, 2.1666f, 2.1666f);
	glVertex3f(4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(0.5f, 0.0f);
	glNormal3f(4.5f, 2.1666f, 2.1666f);
	glVertex3f(0.0f, 4.3334f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0f, 2.1666f, -4.3334f);
	glVertex3f(4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0f, 2.1666f, -4.3334f);
	glVertex3f(-4.5f, -2.1666f, -2.1666f);
	glTexCoord2f(0.5f, 0.0f);
	glNormal3f(0.0f, 2.1666f, -4.3334f);
	glVertex3f(0.0f, 4.3334f, 0.0f);
	
	glEnd();
}

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	
	look_at_origin();
	draw_axes();
	adjust_modelview_rotation();
	
	glEnable(GL_TEXTURE_2D);
	
	if (viewmode & 1<<2)
		draw_tetrahedron();
	if (viewmode & 1<<1)
		
	if (viewmode & 1<<0)
		
	
	glFlush();
	
	glutSwapBuffers();
}

int init_render() {
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
	glEnable(GL_CULL_FACE);
	
	TGAimage* texture_ptr = read_tga("sample.tga");
	if (texture_ptr != NULL) {
		TGAimage texture = *texture_ptr;
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 texture.image_components,
					 texture.header.image_width,
					 texture.header.image_height,
					 0, texture.image_format,
					 GL_UNSIGNED_BYTE,
					 texture.bytes);
		free(texture.bytes);
		
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		fprintf(stderr, "Could not read texture data\n");
		return 1;
	}
	return 0;
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
	
	int init_result = init_render();
	
	if (!init_result) {
		glutMainLoop();
	}
	else {
		return init_result;
	}
	
	return 0;
}
