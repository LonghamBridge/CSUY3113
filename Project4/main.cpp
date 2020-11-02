#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define FIXED_TIMESTEP 0.007f
#define PLATFORM_COUNT 89
#define ENEMY_COUNT 3


struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* enemies;
};

GameState state;
SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;
bool gameIsRunning = true;
float lastTicks = 0;
float accumulator = 0.0f;
int stage = 0;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
        offset + (-0.5f * size), 0.5f * size,
        offset + (-0.5f * size), -0.5f * size,
        offset + (0.5f * size), 0.5f * size,
        offset + (0.5f * size), -0.5f * size,
        offset + (0.5f * size), 0.5f * size,
        offset + (-0.5f * size), -0.5f * size,
            });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    }
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("HW02: Press Space to Start, D, C, K, M to Play!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1920, 1080);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrix);
    projectionMatrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.5f, 0.7f, 0.8f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*------player field------*/
    state.player = new Entity(glm::vec3(-8, -5, 0), glm::vec3(0), 4.0f);
    state.player->entityType = PLAYER;
    state.player->acceleration.y = -12.0f;
    state.player->textureID = LoadTexture("george_0.png");

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
    state.player->width = 0.3f;
    state.player->jumpPower = 6.0f;


    /*------platforms field------*/
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID1 = LoadTexture("platformPack_tile001.png");
    GLuint platformTextureID2 = LoadTexture("platformPack_tile040.png");
    GLuint platformTextureID3 = LoadTexture("platformPack_tile025.png");

    for (int i = 0; i <= 27; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID1;
        state.platforms[i].position = glm::vec3(i - 15.5, -8.5, 0);
    }

    for (int i = 28; i <= 44; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID2;
        state.platforms[i].position = glm::vec3(-15.5, i - 35.5, 0);
    }

    for (int i = 45; i <= 62; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID2;
        state.platforms[i].position = glm::vec3(15.5, i - 53.5, 0);
    }

    for (int i = 63; i <= 88; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID3;
        state.platforms[i].position = glm::vec3(i - 73.5, 4, 0);
    }

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }

    /*------enemies field------*/
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("ctg.png");

    for (int i = 0; i < 3; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration.y = -12.0f;
        state.enemies[i].speed = 2;
    }

    state.enemies[0].position = glm::vec3(7, -5, 0);
    state.enemies[0].aiType = SLEEPER;
    state.enemies[0].aiState = IDLE;

    state.enemies[1].position = glm::vec3(3, 5, 0);
    state.enemies[1].movement.x = 1;
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = IDLE;

    state.enemies[2].position = glm::vec3(2, -4, 0);
    state.enemies[2].aiType = JUMPER;
    state.enemies[2].aiState = MOVING;
    state.enemies[2].jumpPower = 8;

}

void ProcessInput() {
    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (state.player->collidedBottom)
                    state.player->jump = true;
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
        
    }

    if (keys[SDL_SCANCODE_A]) {
        state.player->movement.x = -1;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->movement.x = 1;
        state.player->animIndices = state.player->animRight;
    }


    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
}

void Update() {

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {

        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        for (int i = 0; i < ENEMY_COUNT; i++)
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    DrawText(&program, LoadTexture("font1.png"), "Jump Power:", 0.5f, -0.2, glm::vec3(-14, 8.3, 0));
    if (state.player->jumpPower == 6.0f)
        DrawText(&program, LoadTexture("font1.png"), "+", 0.5f, -0.2, glm::vec3(-10.5, 8.3, 0));
    else if (state.player->jumpPower == 9.0f)
        DrawText(&program, LoadTexture("font1.png"), "++", 0.5f, -0.2, glm::vec3(-10.5, 8.3, 0));
    else
        DrawText(&program, LoadTexture("font1.png"), "+++", 0.5f, -0.2, glm::vec3(-10.5, 8.3, 0));

    if (state.player->isActive == false)
        DrawText(&program, LoadTexture("font1.png"), "You Lose", 1.25f, -0.2, glm::vec3(-3.5, 0, 0));
    if (state.enemies[0].isActive == false && state.enemies[1].isActive == false && state.enemies[2].isActive == false)
        DrawText(&program, LoadTexture("font1.png"), "You Win", 1.25f, -0.2, glm::vec3(-3.5, 0, 0));

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}