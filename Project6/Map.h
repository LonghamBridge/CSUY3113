#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Map {
	int width;
	int height;
	unsigned int* levelData;
	GLuint textureID;
	float tile_size;
	int tile_count_x;
	int tile_count_y;
	std::vector<float> vertices;
	std::vector<float> texCoords;
	float left_bound, right_bound, top_bound, bottom_bound;
public:
	Map(int, int, unsigned int*, GLuint, float, int, int);
	void Build();
	void Render(ShaderProgram*);
	bool IsSolid(glm::vec3, float*, float*);
};