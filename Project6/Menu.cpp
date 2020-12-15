
#include "Menu.h"
#define MENU_WIDTH 20
#define MENU_HEIGHT 9
#define MENU_ENEMY_COUNT 4

unsigned int menu_data[] =
{
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

void Menu::Initialize(int lives) {

    state.nextScene = -1;
    isMenu = true;

    state.player = new Entity(glm::vec3(3, -2, 0), glm::vec3(0), 4.0f);
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("character.png");
    state.player->isActive = false;

    fontID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tilemap.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 14, 10);

    state.enemies = new Entity[MENU_ENEMY_COUNT];
    GLuint snakeTextureID = Util::LoadTexture("snake.png");
    GLuint batTextureID = Util::LoadTexture("bat.png");
    GLuint spiderTextureID = Util::LoadTexture("spider.png");
    GLuint potionsTextureID = Util::LoadTexture("potions.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = snakeTextureID;
    state.enemies[0].position = glm::vec3(2, -2, 0);
    state.enemies[0].aiType = SNAKE;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = batTextureID;
    state.enemies[1].position = glm::vec3(2, -3, 0);
    state.enemies[1].aiType = BAT;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = spiderTextureID;
    state.enemies[2].position = glm::vec3(2, -4, 0);
    state.enemies[2].aiType = SPIDER;

    state.enemies[3].entityType = ENEMY;
    state.enemies[3].textureID = potionsTextureID;
    state.enemies[3].position = glm::vec3(2, -5, 0);
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, NULL, 0);
    for (int i = 0; i < MENU_ENEMY_COUNT; i++)
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, MENU_ENEMY_COUNT);
}

void Menu::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < MENU_ENEMY_COUNT; i++)
        state.enemies[i].Render(program);
    Util::DrawText(program, fontID, "TOWER OF THE SORCERER", 0.75, -0.25, glm::vec3(3, -0.75, 0));
    Util::DrawText(program, fontID, "THIS MONSTER DEALS A FIXED ", 0.5, -0.25, glm::vec3(3.5, -1.75, 0));
    Util::DrawText(program, fontID, "AMOUNT OF DAMAGE TO YOU", 0.5, -0.25, glm::vec3(3.5, -2.25, 0));
    Util::DrawText(program, fontID, "THIS MONSTER DEALS DAMAGE", 0.5, -0.25, glm::vec3(3.5, -2.75, 0));
    Util::DrawText(program, fontID, "BASED ON YOUR CURRENT HP", 0.5, -0.25, glm::vec3(3.5, -3.25, 0));
    Util::DrawText(program, fontID, "FINAL BOSS: YOU NEED TO HAVE AT LEAST ", 0.5, -0.25, glm::vec3(3.5, -3.75, 0));
    Util::DrawText(program, fontID, "2000HP TO BEAT IT AND WIN THE GAME", 0.5, -0.25, glm::vec3(3.5, -4.25, 0));
    Util::DrawText(program, fontID, "COLLECT POTIONS IN THE WORLD", 0.5, -0.25, glm::vec3(3.5, -4.75, 0));
    Util::DrawText(program, fontID, "TO INCREASE CHARACTER'S HP", 0.5, -0.25, glm::vec3(3.5, -5.25, 0));
    Util::DrawText(program, fontID, "WASD TO MOVE, COLLIDE WITH MONSTER TO BATTLE", 0.5, -0.25, glm::vec3(2.5, -6.5, 0));
    Util::DrawText(program, fontID, "PRESS ENTER TO START, FIND THE KEY AND ESCAPE! GOOD LUCK!", 0.5, -0.25, glm::vec3(1, -7, 0));
}