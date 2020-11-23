
#include "Level1.h"
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 9
#define LEVEL1_ENEMY_COUNT 1

unsigned int level1_data[] =
    {
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        3, 1, 1, 1, 1, 0, 0, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
        3, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1
    };

void Level1::Initialize(int lives) {

    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	
    /*------player field------*/
    state.player = new Entity(glm::vec3(3, 0, 0), glm::vec3(0), 4.0f);
    state.player->entityType = PLAYER;
    state.player->acceleration.y = -12.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->lives = lives;

    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };
    state.player->animIndices = state.player->animDown;

    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->jumpPower = 6.0f;


    /*------enemies field------*/
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration.y = -12.0f;
        state.enemies[i].speed = 2;
    }

    state.enemies[0].position = glm::vec3(12, 2, 0);
    state.enemies[0].aiType = SLEEPER;
    state.enemies[0].aiState = IDLE;

}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    if (state.player->position.x > 18) state.nextScene = 1;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
        state.enemies[i].Render(program);
}