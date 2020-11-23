
#include "Level3.h"
#define LEVEL3_WIDTH 20
#define LEVEL3_HEIGHT 9
#define LEVEL3_ENEMY_COUNT 1
#define LEVEL3_ITEM_COUNT 1

unsigned int level3_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3,
    0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize(int lives) {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

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
    state.player->jumpPower = 7.5f;

    /*------item field------*/
    state.item = new Entity[LEVEL3_ITEM_COUNT];
    GLuint itemTextureID = Util::LoadTexture("platformPack_item001.png");
    state.item[0].entityType = ITEM;
    state.item[0].textureID = itemTextureID;
    state.item[0].position = glm::vec3(7, 0, 0);
    state.item[0].modelMatrix = glm::translate(state.item[0].modelMatrix, state.item[0].position);

    /*------enemies field------*/
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("boss.png");

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration.y = -12.0f;
        state.enemies[i].speed = 2;
    }

    state.enemies[0].position = glm::vec3(16, -3, 0);
    state.enemies[0].aiType = SLEEPER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 4;

}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);
    if (state.player->position.x < -0.5) state.player->position.x = -0.5;
    if (state.player->position.x > 17) state.nextScene = 4;
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);
    if (state.player->position.y > -0.5 && state.player->position.x > 6.5 && state.player->position.x < 7.5) {
        state.player->speed = 5;
        state.item[0].isActive = false;
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.item[0].Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
        state.enemies[i].Render(program);
}