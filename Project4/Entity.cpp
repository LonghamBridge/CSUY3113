
#include "Entity.h"

Entity::Entity() {
	modelMatrix = glm::mat4(1.0f);
	position = glm::vec3(0);
	movement = glm::vec3(0);
	velocity = glm::vec3(0);
	acceleration = glm::vec3(0);
	speed = 0;
}

Entity::Entity(glm::vec3 p, glm::vec3 m, float v) {
	modelMatrix = glm::mat4(1.0f);
	position = p;
	movement = m;
	velocity = v * m;
	acceleration = glm::vec3(0);
	speed = v;
}

bool Entity::CheckCollision(Entity* other) {
	if (this == other) return false;
	bool b = isActive && other->isActive && (fabs(position.x - other->position.x) - ((width + other->width) / 2.0f) < 0) && (fabs(position.y - other->position.y) - ((height + other->height) / 2.0f) < 0);
	if (b) {
		lastCollision = other->entityType;
		return true;
	}
	return false;
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount) {
	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];
		if (CheckCollision(object)) {
			float xdist = fabs(position.x - object->position.x);
			float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
				if (object->entityType == ENEMY)
					isActive = false;
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
				if (object->entityType == ENEMY)
					isActive = false;
			}
		}
	}
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];
		if (CheckCollision(object)) {
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
				if (object->entityType == ENEMY)
					isActive = false;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
				if (object->entityType == ENEMY) {
					object->isActive = false;
					jumpPower += 3;
				}

			}
		}
	}
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index) {
	float u = (float)(index % animCols) / (float)animCols;
	float v = (float)(index / animCols) / (float)animRows;

	float width = 1.0f / (float)animCols;
	float height = 1.0f / (float)animRows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
	u, v + height, u + width, v, u, v };

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::AIWalker() {
	if (position.x >= 7)
		movement.x = -1;
	if (position.x <= -2)
		movement.x = 1;
}

void Entity::AISleeper(Entity* player) {
	switch (aiState) {
	case IDLE:
		if (glm::distance(position, player->position) < 3)
			aiState = MOVING;
		break;
	case MOVING:
		movement.x = ((player->position.x < position.x) ? -1 : 1);
		break;
	case ATTACKING:
		break;
	}
}

void Entity::AIJumper() {
	switch (aiState) {
	case MOVING:
		if (collidedBottom)
			jump = true;
		break;
	}
}

void Entity::AI(Entity* player) {
	switch (aiType) {
	case WALKER:
		AIWalker();
		break;
	case SLEEPER:
		AISleeper(player);
		break;
	case JUMPER:
		AIJumper();
		break;
	}
}

void Entity::Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enimies, int enimyCount) {

	if (!isActive) return;

	if (position.y <= -9)
		position.y = 9;

	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	
	if (animIndices != NULL) {
		if (glm::length(movement) != 0) {
			animTime += deltaTime;
			if (animTime >= 0.25f) {
				animTime = 0.0f;
				animIndex++;
				if (animIndex >= animFrames)
					animIndex = 0;
			}
		} else {
			animIndex = 0;
		}
	}

	if (jump) {
		jump = false;
		velocity.y = jumpPower;
	}
	
	velocity.x = speed * movement.x;
	velocity += acceleration * deltaTime;

	position.y += velocity.y * deltaTime; 
	CheckCollisionsY(platforms, platformCount);

	position.x += velocity.x * deltaTime; 
	CheckCollisionsX(platforms, platformCount);

	CheckCollisionsY(enimies, enimyCount);
	CheckCollisionsX(enimies, enimyCount);

	if (entityType == ENEMY) {
		AI(player);
	}

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program) {

	if (!isActive) return;

	program->SetModelMatrix(modelMatrix);

	if (animIndices != NULL) {
		DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
		return;
	}

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}