#include "Effects.h"
#include "Util.h"

Effects::Effects(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, EffectType effectType) {
	if (effectType == HEALTH)
		program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	else
		program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	fontID = Util::LoadTexture("font1.png");
	alpha = 0;
	speed = 1;
	viewOffset = glm::vec3(0);
}

void Effects::DrawOverlay() {
	glUseProgram(program.programID);
	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);
}

void Effects::Start(EffectType effectType) {
	currentEffect = effectType;
	switch (currentEffect) {
	case NONE:
		break;
	case FADEIN:
		alpha = 1.0f;
		break;
	case FADEOUT:
		alpha = 0.0f;
		break;
	case BLOOD:
		alpha = 0.8f;
		break;
	case SHAKE:
		timeLeft = 0.3f;
		break;
	}
}
void Effects::Update(float deltaTime) {
	switch (currentEffect) {
	case NONE:
		break;
	case FADEIN:
		alpha -= speed * deltaTime;
		if (alpha <= 0) currentEffect = NONE;
		break;
	case FADEOUT:
		alpha += speed * deltaTime;
		break;
	case BLOOD:
		alpha -= 2 * deltaTime;
		if (alpha <= 0) currentEffect = NONE;
		break;
	case SHAKE:
		timeLeft -= deltaTime * speed;
		if (timeLeft <= 0) {
			viewOffset = glm::vec3(0);
			currentEffect = NONE;
		} else {
			float max = 0.1f;
			float min = -0.1f;
			float x = ((float)rand() / RAND_MAX) * (max - min) + min;
			float y = ((float)rand() / RAND_MAX) * (max - min) + min;
			viewOffset = glm::vec3(x, y, 0);
		}
	}
}
void Effects::Render() {
	glm::mat4 modelMatrix = glm::mat4(1);
	switch (currentEffect) {
	case NONE:
		return;
		break;
	case FADEIN:
	case FADEOUT:
		modelMatrix = glm::scale(modelMatrix, glm::vec3(16, 9, 1));
		program.SetModelMatrix(modelMatrix);
		program.SetColor(0, 0, 0, alpha);
		DrawOverlay();
		break;
	case BLOOD:
		modelMatrix = glm::scale(modelMatrix, glm::vec3(16, 9, 1));
		program.SetModelMatrix(modelMatrix);
		program.SetColor(1, 0, 0, alpha);
		DrawOverlay();
		break;
	case SHAKE:
		break;
	case HEALTH:
		Util::DrawText(&program, fontID, std::to_string((int)speed), 1, 0, glm::vec3(0, 0, 0));
		break;
	}
}