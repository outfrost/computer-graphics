#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef float point3f[3];

const float AXIS_RADIUS = 5.0f;
const float MIN_FOV_ORTHO = 10.0f;
const unsigned int EGG_SUBDIVISIONS = 20u;

short viewmode = 0x1;

point3f * egg_vertices = NULL;
point3f * vertex_colors = NULL;

point3f * get_vertex(int i, int k) {
	return &(egg_vertices[i * EGG_SUBDIVISIONS + k]);
}

point3f * get_vertex_color(point3f * vertex) {
	return vertex_colors + (vertex - egg_vertices);
}

void create_vertex(float u, float v, point3f * result) {
	(*result)[0] = (-  90.0f * powf(u, 5.0f)
			+ 225.0f * powf(u, 4.0f)
			- 270.0f * powf(u, 3.0f)
			+ 180.0f * powf(u, 2.0f)
			-  45.0f * u) * cosf (M_PI * v);
	
	(*result)[1] =    160.0f * powf(u, 4.0f)
			- 320.0f * powf(u, 3.0f)
			+ 160.0f * powf(u, 2.0f)
			- 5.0f;
	
	(*result)[2] = (-  90.0f * powf(u, 5.0f)
			+ 225.0f * powf(u, 4.0f)
			- 270.0f * powf(u, 3.0f)
			+ 180.0f * powf(u, 2.0f)
			-  45.0f * u) * sinf(M_PI * v);
}

void create_vertex_color(point3f * vertex, point3f * result) {
	for (int i = 0; i < 3; ++i) {
		(*result)[i] = ((*vertex)[i] + 5.0f) / 10.0f;
	}
}

void compute_egg_vertices() {
	egg_vertices = realloc(egg_vertices, EGG_SUBDIVISIONS * EGG_SUBDIVISIONS * sizeof(point3f));
	vertex_colors = realloc(vertex_colors, EGG_SUBDIVISIONS * EGG_SUBDIVISIONS * sizeof(point3f));
	
	for (int i = 0; i < EGG_SUBDIVISIONS; ++i) {
		float u = (1.0f / EGG_SUBDIVISIONS) * i;
		for (int k = 0; k < EGG_SUBDIVISIONS; ++k) {
			float v = (1.0f / EGG_SUBDIVISIONS) * k;
			point3f * vertex = get_vertex(i, k);
			create_vertex(u, v, vertex);
			create_vertex_color(vertex, get_vertex_color(vertex));
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

void draw_egg_vertices() {
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

void edge(point3f * vertex, point3f * next_vertex) {
	glVertex3f((*vertex)[0], (*vertex)[1], (*vertex)[2]);
	glVertex3f((*next_vertex)[0], (*next_vertex)[1], (*next_vertex)[2]);
}

void face(point3f * v1, point3f * v2, point3f * v3) {
	point3f * v_color = get_vertex_color(v1);
	glColor3f((*v_color)[0], (*v_color)[1], (*v_color)[2]);
	glVertex3f((*v1)[0], (*v1)[1], (*v1)[2]);
	v_color = get_vertex_color(v2);
	glColor3f((*v_color)[0], (*v_color)[1], (*v_color)[2]);
	glVertex3f((*v2)[0], (*v2)[1], (*v2)[2]);
	v_color = get_vertex_color(v3);
	glColor3f((*v_color)[0], (*v_color)[1], (*v_color)[2]);
	glVertex3f((*v3)[0], (*v3)[1], (*v3)[2]);
}

void draw_egg_edges() {
	glColor3f(1.0f, 1.0f, 0.5f);
	glBegin(GL_LINES);
	for (int i = 0, i_next = 1; i < EGG_SUBDIVISIONS; ++i, ++i_next) {
		if (i_next == EGG_SUBDIVISIONS)
			i_next = 0;
		for (int k = 0, k_next = 1; k < EGG_SUBDIVISIONS; ++k, ++k_next) {
			point3f * vertex = get_vertex(i, k);
			edge(vertex, get_vertex(i_next, k));
			if (k_next == EGG_SUBDIVISIONS) {
				edge(vertex, get_vertex(EGG_SUBDIVISIONS - i, 0));
				edge(vertex, get_vertex(EGG_SUBDIVISIONS - i_next, 0));
			}
			else {
				edge(vertex, get_vertex(i, k_next));
				edge(vertex, get_vertex(i_next, k_next));
			}
		}
	}
	glEnd();
}

void draw_egg_faces() {
	glColor3f(0.0f, 0.6f, 0.4f);
	glBegin(GL_TRIANGLES);
	for (int i = 0, i_next = 1; i < EGG_SUBDIVISIONS; ++i, ++i_next) {
		if (i_next == EGG_SUBDIVISIONS)
			i_next = 0;
		for (int k = 0, k_next = 1; k < EGG_SUBDIVISIONS; ++k, ++k_next) {
			point3f * v00 = get_vertex(i, k);
			point3f * v01 = k_next == EGG_SUBDIVISIONS ?
					  get_vertex(EGG_SUBDIVISIONS - i, 0)
					: get_vertex(i, k_next);
			point3f * v10 = get_vertex(i_next, k);
			point3f * v11 = k_next == EGG_SUBDIVISIONS ?
					  get_vertex(EGG_SUBDIVISIONS - i_next, 0)
					: get_vertex(i_next, k_next);
			face(v00, v01, v11);
			face(v00, v10, v11);
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
	
	if (viewmode & 1<<2)
		draw_egg_faces();
	if (viewmode & 1<<1)
		draw_egg_edges();
	if (viewmode & 1<<0)
		draw_egg_vertices();
	
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
		glOrtho(-MIN_FOV_ORTHO, MIN_FOV_ORTHO, -MIN_FOV_ORTHO/aspect_ratio, MIN_FOV_ORTHO/aspect_ratio, 128.0, -128.0);
	else
		glOrtho(-MIN_FOV_ORTHO*aspect_ratio, MIN_FOV_ORTHO*aspect_ratio, -MIN_FOV_ORTHO, MIN_FOV_ORTHO, 128.0, -128.0);
	
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	
	// Set the coordinate system and bounding space again
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

int main(int argc, char** argv) {
	compute_egg_vertices();
	
	glutInit(&argc, argv);
	
	glutInitWindowSize(640, 480);
	
	// Set display mode: single frame buffer, RGBA colour model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	
	// Create a window (stage) with a title
	glutCreateWindow("3D coordinate space");
	
	// Set rendering callback to render_scene()
	// GLUT will call this function whenever a stage redraw is necessary
	glutDisplayFunc(render_scene);
	
	// Set the stage reshape handling function to resize_stage()
	glutReshapeFunc(resize_stage);
	
	// Set the keyboard event handling function to key_pressed()
	glutKeyboardFunc(key_pressed);
	
	init_render();
	
	// Enable the removal of invisible surfaces
	glEnable(GL_DEPTH_TEST);
	
	// Start the GLUT framework main loop
	glutMainLoop();
}
