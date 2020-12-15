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

enum EffectType { NONE, FADEIN, FADEOUT, BLOOD, SHAKE, HEALTH };

class Effects {
	ShaderProgram program;
	float alpha;
	float speed;
	float timeLeft;
	EffectType currentEffect;
	GLuint fontID;

public:
	glm::vec3 viewOffset;
	Effects(glm::mat4, glm::mat4, EffectType);
	void DrawOverlay();
	void Start(EffectType);
	void Update(float);
	void Render();
};