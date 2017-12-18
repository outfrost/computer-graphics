#include <errno.h>

#pragma pack(push, 1)
typedef struct {
	GLubyte id_length;
	GLbyte color_map_type;
	GLbyte image_type;
	GLushort color_map_start;
	GLushort color_map_length;
	GLubyte color_map_bpp;
	GLushort x_origin;
	GLushort y_origin;
	GLushort image_width;
	GLushort image_height;
	GLubyte image_bpp;
	GLbyte image_descriptor;
} TGAheader;
#pragma pack(pop)

typedef struct {
	TGAheader header;
	GLenum image_format;
	GLint image_components;
	GLbyte* bytes;
} TGAimage;

TGAimage* read_tga(const char* path) {
	FILE* tga_file = fopen(path, "rb");
	if (tga_file == NULL) {
		fprintf(stderr, "Error %d opening texture file\n", errno);
		return NULL;
	}
	
	TGAheader header;
	
	fread(&header, sizeof(TGAheader), 1, tga_file);
	
	GLenum image_format;
	GLint image_components;
	
	if (header.image_bpp == 32) {
		image_format = GL_BGRA_EXT;
		image_components = GL_RGBA8;
	}
	else if (header.image_bpp == 24) {
		image_format = GL_BGR_EXT;
		image_components = GL_RGB8;
	}
	else if (header.image_bpp == 8) {
		image_format = GL_LUMINANCE;
		image_components = GL_LUMINANCE8;
	}
	else {
		fprintf(stderr, "Unsupported TGA bpp format\n");
		return NULL;
	}
	
	unsigned long image_size = header.image_width * header.image_height * (header.image_bpp / 8);
	
	printf("Image size: %lu\n", image_size);
	
	GLbyte* bytes = malloc(image_size * sizeof(GLbyte));
	if (bytes == NULL) {
		fprintf(stderr, "Unable to allocate space for TGA pixel data: Error %d\n", errno);
		return NULL;
	}
	
	if (fread(bytes, image_size, 1, tga_file) != 1) {
		fprintf(stderr, "Cannot read TGA pixel data (possibly a compressed image)");
		free(bytes);
		return NULL;
	}
	
	fclose(tga_file);
	
	TGAimage* image = malloc(sizeof(TGAimage));
	if (image == NULL) {
		fprintf(stderr, "Unable to allocate space for TGA struct: Error %d\n", errno);
		return NULL;
	}
	
	(*image).header = header;
	(*image).image_format = image_format;
	(*image).image_components = image_components;
	(*image).bytes = bytes;
	
	return image;
}
