
#include "Level1.h"
#define LEVEL1_WIDTH 40
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 41

unsigned int level1_data[] =
    {
        0, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1,  1,  1,  1, 1, 0, 1, 1, 1, 3,
        14,5, 5, 5, 5, 5, 5, 5, 17,5, 5, 5, 5, 5, 5, 5, 5, 14,5, 5, 5, 5, 14,5, 5, 5, 5, 5, 5, 17,5,  5,  5,  5, 5, 14,5, 5, 5, 17,
        14,5, 5, 5, 5, 5, 5, 5, 17,5, 5, 5, 5, 5, 5, 17,5, 14,5, 5, 5, 5, 28,1, 1, 1, 3, 5, 5, 17,5,  5,  5,  5, 5, 14,5, 5, 5, 17,
        14,5, 5, 5, 5, 5, 5, 5, 17,1, 1, 1, 1, 1, 1, 31,5, 14,5, 5, 5, 5, 5, 5, 5, 5, 17,5, 5, 17,5,  5,  5,  5, 5, 14,5, 5, 5, 17,
        28,1, 29,1, 5, 5, 5, 1, 31,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 14,5, 5, 5, 17,5, 5, 17,5,109,110,111, 5, 28,1, 5, 1, 31,
        14,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 17,5, 14,5, 5, 5, 5, 28,1, 1, 1, 31,5, 5, 5, 5,123,124,125, 5, 5, 5, 5, 5, 17,
        14,5, 5, 5, 5, 5, 5, 5, 17,5, 5, 5, 5, 5, 5, 17,5, 14,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 17,5,137,138,139, 5, 5, 5, 5, 5, 17,
        28,1, 1, 1, 1, 1, 1, 1, 31,1, 1, 1, 1, 1, 1, 31,1, 28,1, 1, 1, 1, 1, 1, 1, 1, 1, 30,1, 31,1,  1,  1,  1, 1, 1, 1, 1, 1, 31,
        
    };

void Level1::Initialize(int lives) {
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tilemap.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 14, 10);

    fontID = Util::LoadTexture("font1.png");

    /*------player field------*/
    state.player = new Entity(glm::vec3(3, -2, 0), glm::vec3(0), 4.0f);
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("character.png");
    state.player->lives = lives;


    state.player->height = 0.8f;
    state.player->width = 0.4f;


    /*------enemies field------*/
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint snakeTextureID = Util::LoadTexture("snake.png");
    GLuint batTextureID = Util::LoadTexture("bat.png");
    GLuint spiderTextureID = Util::LoadTexture("spider.png");

    GLuint redTextureID = Util::LoadTexture("red.png");
    GLuint blueTextureID = Util::LoadTexture("blue.png");
    GLuint greenTextureID = Util::LoadTexture("green.png");
    GLuint yellowTextureID = Util::LoadTexture("yellow.png");

    GLuint keyTextureID = Util::LoadTexture("key.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 3;
        state.enemies[i].height = 0.9;
        state.enemies[i].width = 0.9;
    }
    
    state.enemies[0].textureID = spiderTextureID;
    state.enemies[0].position = glm::vec3(24, -1, 0);
    state.enemies[0].aiType = SPIDER;

    state.enemies[1].textureID = snakeTextureID;
    state.enemies[1].position = glm::vec3(8, -5, 0);
    state.enemies[1].aiType = SNAKE;

    state.enemies[2].textureID = snakeTextureID;
    state.enemies[2].position = glm::vec3(22, -3, 0);
    state.enemies[2].aiType = SNAKE;

    state.enemies[3].textureID = snakeTextureID;
    state.enemies[3].position = glm::vec3(19, -5, 0);
    state.enemies[3].aiType = SNAKE;

    state.enemies[4].textureID = snakeTextureID;
    state.enemies[4].position = glm::vec3(13, -1, 0);
    state.enemies[4].aiType = SNAKE;

    state.enemies[5].textureID = snakeTextureID;
    state.enemies[5].position = glm::vec3(20, -6, 0);
    state.enemies[5].aiType = SNAKE;

    state.enemies[6].textureID = batTextureID;
    state.enemies[6].position = glm::vec3(6, -4, 0);
    state.enemies[6].aiType = BAT;

    state.enemies[7].textureID = batTextureID;
    state.enemies[7].position = glm::vec3(13, -5, 0);
    state.enemies[7].aiType = BAT;

    state.enemies[8].textureID = batTextureID;
    state.enemies[8].position = glm::vec3(16, -5, 0);
    state.enemies[8].aiType = BAT;

    state.enemies[9].textureID = batTextureID;
    state.enemies[9].position = glm::vec3(18, -5, 0);
    state.enemies[9].aiType = BAT;

    state.enemies[10].textureID = batTextureID;
    state.enemies[10].position = glm::vec3(21, -5, 0);
    state.enemies[10].aiType = BAT;

    state.enemies[11].textureID = batTextureID;
    state.enemies[11].position = glm::vec3(27, -5, 0);
    state.enemies[11].aiType = BAT;

    state.enemies[12].textureID = redTextureID;
    state.enemies[12].position = glm::vec3(1, -6, 0);
    state.enemies[12].aiType = RED;

    state.enemies[13].textureID = redTextureID;
    state.enemies[13].position = glm::vec3(25, -4, 0);
    state.enemies[13].aiType = RED;

    state.enemies[14].textureID = blueTextureID;
    state.enemies[14].position = glm::vec3(7, -2, 0);
    state.enemies[14].aiType = BLUE;

    state.enemies[15].textureID = blueTextureID;
    state.enemies[15].position = glm::vec3(14, -6, 0);
    state.enemies[15].aiType = BLUE;

    state.enemies[16].textureID = blueTextureID;
    state.enemies[16].position = glm::vec3(24, -4, 0);
    state.enemies[16].aiType = BLUE;

    state.enemies[17].textureID = greenTextureID;
    state.enemies[17].position = glm::vec3(13, -2, 0);
    state.enemies[17].aiType = GREEN;

    state.enemies[18].textureID = greenTextureID;
    state.enemies[18].position = glm::vec3(12, -2, 0);
    state.enemies[18].aiType = GREEN;

    state.enemies[19].textureID = greenTextureID;
    state.enemies[19].position = glm::vec3(11, -2, 0);
    state.enemies[19].aiType = GREEN;

    state.enemies[20].textureID = yellowTextureID;
    state.enemies[20].position = glm::vec3(7, -1, 0);
    state.enemies[20].aiType = YELLOW;

    state.enemies[21].textureID = yellowTextureID;
    state.enemies[21].position = glm::vec3(7, -3, 0);
    state.enemies[21].aiType = YELLOW;

    state.enemies[22].textureID = yellowTextureID;
    state.enemies[22].position = glm::vec3(20, -1, 0);
    state.enemies[22].aiType = YELLOW;

    state.enemies[23].textureID = yellowTextureID;
    state.enemies[23].position = glm::vec3(19, -1, 0);
    state.enemies[23].aiType = YELLOW;

    state.enemies[24].textureID = yellowTextureID;
    state.enemies[24].position = glm::vec3(20, -6, 0);
    state.enemies[24].aiType = YELLOW;

    state.enemies[25].textureID = yellowTextureID;
    state.enemies[25].position = glm::vec3(19, -6, 0);
    state.enemies[25].aiType = YELLOW;

    state.enemies[26].textureID = batTextureID;
    state.enemies[26].position = glm::vec3(3, -5, 0);
    state.enemies[26].aiType = BAT;

    state.enemies[27].textureID = batTextureID;
    state.enemies[27].position = glm::vec3(2, -6, 0);
    state.enemies[27].aiType = BAT;

    state.enemies[28].textureID = batTextureID;
    state.enemies[28].position = glm::vec3(14, -2, 0);
    state.enemies[28].aiType = BAT;

    state.enemies[29].textureID = batTextureID;
    state.enemies[29].position = glm::vec3(23, -4, 0);
    state.enemies[29].aiType = BAT;

    state.enemies[30].textureID = keyTextureID;
    state.enemies[30].position = glm::vec3(23, -1, 0);
    state.enemies[30].aiType = KEY;

    state.enemies[31].textureID = redTextureID;
    state.enemies[31].position = glm::vec3(36, -1, 0);
    state.enemies[31].aiType = RED;

    state.enemies[32].textureID = redTextureID;
    state.enemies[32].position = glm::vec3(38, -1, 0);
    state.enemies[32].aiType = RED;

    state.enemies[33].textureID = batTextureID;
    state.enemies[33].position = glm::vec3(37, -3, 0);
    state.enemies[33].aiType = BAT;

    state.enemies[34].textureID = snakeTextureID;
    state.enemies[34].position = glm::vec3(37, -2, 0);
    state.enemies[34].aiType = SNAKE;

    state.enemies[35].textureID = snakeTextureID;
    state.enemies[35].position = glm::vec3(39, -3, 0);
    state.enemies[35].aiType = SNAKE;

    state.enemies[36].textureID = snakeTextureID;
    state.enemies[36].position = glm::vec3(37, -5, 0);
    state.enemies[36].aiType = SNAKE;

    state.enemies[37].textureID = snakeTextureID;
    state.enemies[37].position = glm::vec3(33, -2, 0);
    state.enemies[37].aiType = SNAKE;

    state.enemies[38].textureID = batTextureID;
    state.enemies[38].position = glm::vec3(32, -3, 0);
    state.enemies[38].aiType = BAT;

    state.enemies[39].textureID = greenTextureID;
    state.enemies[39].position = glm::vec3(31, -1, 0);
    state.enemies[39].aiType = GREEN;

    state.enemies[40].textureID = greenTextureID;
    state.enemies[40].position = glm::vec3(33, -1, 0);
    state.enemies[40].aiType = GREEN;

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        if (state.enemies[i].aiType == BAT)
            state.enemies[i].movement.y = 1;
        else if (state.enemies[i].aiType == SNAKE)
            state.enemies[i].movement.x = 1;
    }

}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
        state.enemies[i].Render(program);
    program->SetViewMatrix(glm::mat4(1.0f));
    if (state.player->key && state.player->position.x <= 27 && state.player->position.y <= -5.5) {
        Util::DrawText(program, fontID, "YOU ESCAPED!!!", 1, 0, glm::vec3(-6.5, -4, 0));
        state.player->escaped = true;
        state.player->isActive = false;
    }
    else if(state.player->isActive)
        Util::DrawText(program, fontID, "HP:" + std::to_string(state.player->lives), 1, 0, glm::vec3(-7, -4, 0));
    else
        Util::DrawText(program, fontID, "YOU DIED!!!", 1, 0, glm::vec3(-5, -4, 0));
}