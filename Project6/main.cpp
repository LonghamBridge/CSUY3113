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
Mix_Music* BGM;
Mix_Chunk* death;

Scene* currentScene;
Scene* sceneList[2];

Effects* effects;
Effects* effects2;

void SwitchToScene(Scene* scene, int lives) {
    currentScene = scene;
    currentScene->Initialize(lives);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("TOWER OF THE SORCERER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1920, 1080);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);      //HEY! My music was not working so I made something fun here...
    BGM = Mix_LoadMUS("bgm.mp3");
    death = Mix_LoadWAV("death.wav");
    Mix_PlayMusic(BGM, -1);
    Mix_VolumeMusic(SDL_MIX_MAXVOLUME / 4);

    viewMatrix = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrix);
    projectionMatrix = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.13f, 0.13f, 0.13f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontID = Util::LoadTexture("font1.png");

    sceneList[0] = new Level1();
    sceneList[1] = new Menu();
    SwitchToScene(sceneList[1], 1000);

    effects = new Effects(projectionMatrix, viewMatrix, BLOOD);
    effects2 = new Effects(projectionMatrix, viewMatrix, FADEOUT);
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
                break;
            case SDLK_RETURN:
                if (currentScene->isMenu)
                    SwitchToScene(sceneList[0], 1000);
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
    }
    else if (keys[SDL_SCANCODE_D]) {
        currentScene->state.player->movement.x = 1;
    }
    if (keys[SDL_SCANCODE_W]) {
        currentScene->state.player->movement.y = 1;
    }
    else if (keys[SDL_SCANCODE_S]) {
        currentScene->state.player->movement.y = -1;
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
    if (currentScene->state.player->position.x > 7.5 && currentScene->state.player->position.x < 31.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4, 0));
    }
    else if (currentScene->state.player->position.x < 7.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-7.5, 4, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-31.5, 4, 0));
    }
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);

    effects->Render();
    effects2->Render();

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeChunk(death);
    Mix_FreeMusic(BGM);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        if (!currentScene->state.player->isActive && stage == 0 && !currentScene->isMenu && !currentScene->state.player->escaped) {
            effects->Start(BLOOD);
            Mix_HaltMusic();
            Mix_PlayChannel(-1, death, 0);
            stage = 1;
        }
        if (currentScene->state.player->escaped) {
            Mix_HaltMusic();
        }
        Render();
    }

    Shutdown();
    return 0;
}