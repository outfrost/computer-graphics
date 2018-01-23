//#include <GL/glut.h>
//#include <math.h>

point3f * egg_vertices = NULL;
point3f * vertex_colors = NULL;
point3f * egg_normals = NULL;

point3f * get_vertex(int i, int k) {
	return &(egg_vertices[i * EGG_SUBDIVISIONS + k]);
}

point3f * get_vertex_color(point3f * vertex) {
	return vertex_colors + (vertex - egg_vertices);
}

point3f * get_vertex_normal(point3f * vertex) {
	return egg_normals + (vertex - egg_vertices);
}

void create_vertex(float u, float v, point3f * result, point3f * normal) {
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
	
	float xu = (- 450.0f * powf(u, 4.0f)
		    + 900.0f * powf(u, 3.0f)
		    - 810.0f * powf(u, 2.0f)
		    + 360.0f * u
		    -  45.0f) * cosf(M_PI * v);
	
	float xv = (   90.0f * powf(u, 5.0f)
		    - 225.0f * powf(u, 4.0f)
		    + 270.0f * powf(u, 3.0f)
		    - 180.0f * powf(u, 2.0f)
		    +  45.0f * u) * M_PI * sinf(M_PI * v);
	
	float yu =   640.0f * powf(u, 3.0f)
		   - 960.0f * powf(u, 2.0f)
		   + 320.0f * u;
	
	float yv = 0.0f;
	
	float zu = (- 450.0f * powf(u, 4.0f)
		    + 900.0f * powf(u, 3.0f)
		    - 810.0f * powf(u, 2.0f)
		    + 360.0f * u
		    -  45.0f) * sinf(M_PI * v);
	
	float zv = (   90.0f * powf(u, 5.0f)
		    - 225.0f * powf(u, 4.0f)
		    + 270.0f * powf(u, 3.0f)
		    - 180.0f * powf(u, 2.0f)
		    +  45.0f * u) * (- M_PI) * cosf(M_PI * v);
	
	(*normal)[0] = yu * zv - zu * yv;
	(*normal)[1] = zu * xv - xu * zv;
	(*normal)[2] = xu * yv - yu * xv;
	if (u > 0.5f) {
		(*normal)[0] *= -1.0f;
		(*normal)[1] *= -1.0f;
		(*normal)[2] *= -1.0f;
	}
}

void create_vertex_color(point3f * vertex, point3f * result) {
	for (int i = 0; i < 3; ++i) {
		(*result)[i] = ((*vertex)[i] + 5.0f) / 10.0f;
	}
}

void compute_egg_vertices() {
	egg_vertices = realloc(egg_vertices,
			       EGG_SUBDIVISIONS * EGG_SUBDIVISIONS
					* sizeof(point3f));
	vertex_colors = realloc(vertex_colors,
				EGG_SUBDIVISIONS * EGG_SUBDIVISIONS
					* sizeof(point3f));
	egg_normals = realloc(egg_normals,
			      EGG_SUBDIVISIONS * EGG_SUBDIVISIONS
					* sizeof(point3f));
	
	for (int i = 0; i < EGG_SUBDIVISIONS; ++i) {
		float u = (1.0f / EGG_SUBDIVISIONS) * i;
		for (int k = 0; k < EGG_SUBDIVISIONS; ++k) {
			float v = (1.0f / EGG_SUBDIVISIONS) * k;
			point3f * vertex = get_vertex(i, k);
			point3f * normal = get_vertex_normal(vertex);
			create_vertex(u, v, vertex, normal);
			create_vertex_color(vertex, get_vertex_color(vertex));
		}
	}
}

void vert(point3f * vertex) {
	memcpy(mat_ambient, get_vertex_color(vertex), sizeof(point3f));
	memcpy(mat_diffuse, get_vertex_color(vertex), sizeof(point3f));
	memcpy(mat_specular, get_vertex_color(vertex), sizeof(point3f));
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	
	point3f * normal = get_vertex_normal(vertex);
	glNormal3f((*normal)[0], (*normal)[1], (*normal)[2]);
	glVertex3f((*vertex)[0], (*vertex)[1], (*vertex)[2]);
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
	vert(vertex);
	vert(next_vertex);
}

void face(point3f * v1, point3f * v2, point3f * v3) {
	vert(v1);
	vert(v2);
	vert(v3);
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
