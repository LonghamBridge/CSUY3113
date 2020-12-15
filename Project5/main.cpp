#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Effects.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Menu.h"

#define FIXED_TIMESTEP 0.007f

SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;
bool gameIsRunning = true;
float lastTicks = 0;
float accumulator = 0.0f;
int stage = 0;
GLuint fontID;
GLuint fontIDRed;
Mix_Music* music1;
Mix_Chunk* jump;

Scene* currentScene;
Scene* sceneList[4];

Effects* effects;
Effects* effects2;

void SwitchToScene(Scene* scene, int lives) {
    currentScene = scene;
    currentScene->Initialize(lives);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Outdoor Dungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1920, 1080);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(88888, MIX_DEFAULT_FORMAT, 2, 4096);      
    music1 = Mix_LoadMUS("dooblydoo.mp3");
    jump = Mix_LoadWAV("jump.wav");
    Mix_PlayMusic(music1, -1);
    Mix_VolumeMusic(SDL_MIX_MAXVOLUME / 4);

    viewMatrix = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrix);
    projectionMatrix = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.5f, 0.7f, 0.8f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontID = Util::LoadTexture("font1.png");
    fontIDRed = Util::LoadTexture("font_red.png");

    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    sceneList[3] = new Menu();
    sceneList[3]->isMenu = true;
    SwitchToScene(sceneList[3], 3);

    //effects = new Effects(projectionMatrix, viewMatrix, HEALTH);
    //effects2 = new Effects(projectionMatrix, viewMatrix, SHAKE);
    //effects->Start(HEALTH);
    //effects2->Start(FADEIN);
}



void ProcessInput() {
    currentScene->state.player->movement = glm::vec3(0);

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
                if (currentScene->state.player->collidedBottom) {
                    currentScene->state.player->jump = true;
                    Mix_PlayChannel(-1, jump, 0);
                    //effects->Start(SHAKE);
                    //effects2->Start(BLOOD);
                }
                break;
            case SDLK_RETURN:
                if (currentScene->isMenu)
                    currentScene->state.nextScene = 0;
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
        
    }

    if (keys[SDL_SCANCODE_A]) {
        currentScene->state.player->movement.x = -1;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_D]) {
        currentScene->state.player->movement.x = 1;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }


    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
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
        currentScene->Update(FIXED_TIMESTEP);
        effects->Update(FIXED_TIMESTEP);
        effects2->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 7.5 && currentScene->state.player->position.x < 10) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4, 0));
    }
    else if (currentScene->state.player->position.x < 7.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-7.5, 4, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-10, 4, 0));
    }
    
    viewMatrix = glm::translate(viewMatrix, effects->viewOffset);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (currentScene->state.player->lives == 0) {
        Util::DrawText(&program, fontID, "YOU LOSE", 1.0, 0.5, glm::vec3(3, -2, 0));
        Mix_HaltMusic();
    }
    if (currentScene->state.nextScene == 4) {
        Util::DrawText(&program, fontID, "YOU WIN", 1.0, 0.5, glm::vec3(5, -2, 0));
        currentScene->state.player->isActive = false;
        Mix_HaltMusic();
    }
    if (currentScene->isMenu) {
        Util::DrawText(&program, fontID, "Welcome to Outdoor Dungeon", 0.5, 0, glm::vec3(2, -1.5, 0));
        Util::DrawText(&program, fontID, "Press A and D to move", 0.5, 0, glm::vec3(3.25, -2.5, 0));
        Util::DrawText(&program, fontID, "Press Space to jump", 0.5, 0, glm::vec3(3.5, -3.5, 0));
        Util::DrawText(&program, fontID, "Press Enter to start", 0.5, 0, glm::vec3(3.25, -4.5, 0));
        Util::DrawText(&program, fontIDRed, "Do not try to kill monsters, or you will die!", 0.5, -0.2, glm::vec3(2.5, -5.5, 0));
    }

    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);
    currentScene->Render(&program);
    
    effects->Render();
    effects2->Render();
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeChunk(jump);
    Mix_FreeMusic(music1);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        if (currentScene->state.nextScene >= 0 && currentScene->state.nextScene != 4) SwitchToScene(sceneList[currentScene->state.nextScene], currentScene->state.player->lives);
        Render();
    }

    Shutdown();
    return 0;
}