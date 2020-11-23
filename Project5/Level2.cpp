
#include "Level2.h"
#define LEVEL2_WIDTH 20
#define LEVEL2_HEIGHT 9
#define LEVEL2_ENEMY_COUNT 5

unsigned int level2_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize(int lives) {
    
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);

    /*------player field------*/
    state.player = new Entity(glm::vec3(0, -6, 0), glm::vec3(0), 4.0f);
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

    state.player->height = 0.8;
    state.player->width = 0.8;
    state.player->jumpPower = 6.0f;


    /*------enemies field------*/
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration.y = -12.0f;
        state.enemies[i].speed = 2;
    }

    state.enemies[0].position = glm::vec3(5.5, -3, 0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = MOVING;
    state.enemies[0].jumpPower = 7;

    state.enemies[1].position = glm::vec3(7, -3, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = MOVING;
    state.enemies[1].jumpPower = 9;

    state.enemies[2].position = glm::vec3(8.5, -3, 0);
    state.enemies[2].aiType = JUMPER;
    state.enemies[2].aiState = MOVING;
    state.enemies[2].jumpPower = 11;

    state.enemies[3].position = glm::vec3(10, -3, 0);
    state.enemies[3].aiType = JUMPER;
    state.enemies[3].aiState = MOVING;
    state.enemies[3].jumpPower = 8;

    state.enemies[4].position = glm::vec3(11.5, -3, 0);
    state.enemies[4].aiType = JUMPER;
    state.enemies[4].aiState = MOVING;
    state.enemies[4].jumpPower = 10;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT);
    if (state.player->position.x < -0.5) state.player->position.x = -0.5;
    if (state.player->position.x > 18) state.nextScene = 2;
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++)
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT);
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++)
        state.enemies[i].Render(program);
}