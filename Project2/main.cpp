#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;
bool gameIsRunning = true;
float lastTicks = 0.0f;

/*------Paddle1 Field------*/
glm::mat4 paddle1Matrix = glm::mat4(1.0f);
glm::vec3 paddle1Position = glm::vec3(7.8, 0, 0);
glm::vec3 paddle1Movement;
GLuint paddle1TextureID;
float paddle1Speed = 1.0f;
float paddle1Height = 1.6f;
float paddle1Width = 0.4f;

/*------Paddle2 Field------*/
glm::mat4 paddle2Matrix = glm::mat4(1.0f);
glm::vec3 paddle2Position = glm::vec3(-7.8, 0, 0);
glm::vec3 paddle2Movement;
GLuint paddle2TextureID;
float paddle2Speed = 1.0f;
float paddle2Height = 1.6f;
float paddle2Width = 0.4f;

/*------Ball Field------*/
glm::mat4 ballMatrix = glm::mat4(1.0f);
glm::vec3 ballPosition = glm::vec3(0, 0, 0);
glm::vec3 ballMovement;
GLuint ballTextureID;
float ballSpeed = 1.0f;
float ballHeight = 0.4f;
float ballWidth = 0.4f;

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

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("HW02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1920, 1080);
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrix);
    projectionMatrix = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void ProcessInput() {
    paddle1Movement = glm::vec3(0);
    paddle2Movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_K]) {
        if (paddle1Position.y < 3.7)
            paddle1Movement.y = 10.0f;
    }
    else if (keys[SDL_SCANCODE_M]) {
        if (paddle1Position.y > -3.7)
            paddle1Movement.y = -10.0f;
    }

    if (keys[SDL_SCANCODE_D]) {
        if (paddle2Position.y < 3.7)
            paddle2Movement.y = 10.0f;
    }
    else if (keys[SDL_SCANCODE_C]) {
        if (paddle2Position.y > -3.7)
            paddle2Movement.y = -10.0f;
    }

    

}

void Update() {
    /*------Update Ticks------*/
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    /*------Update Movements------*/
    //Paddle1
    paddle1Position += paddle1Movement * paddle1Speed * deltaTime;
    paddle1Matrix = glm::mat4(1.0f);
    paddle1Matrix = glm::translate(paddle1Matrix, paddle1Position);

    //Paddle2
    paddle2Position += paddle1Movement * paddle1Speed * deltaTime;
    paddle2Matrix = glm::mat4(1.0f);
    paddle2Matrix = glm::translate(paddle1Matrix, paddle1Position);

}


void Render() {
    float paddle1[] = { -0.2, -0.8, 0.2, -0.8, 0.2, 0.8, -0.2, -0.8, 0.2, 0.8, -0.2, 0.8 };
    float paddle2[] = { -0.2, -0.8, 0.2, -0.8, 0.2, 0.8, -0.2, -0.8, 0.2, 0.8, -0.2, 0.8 };
    float ball[] = { -0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2 };


    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle1);
    glEnableVertexAttribArray(program.positionAttribute);


    /*-------DRAW PLAYER-------*/
    program.SetModelMatrix(paddle1Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(paddle2Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);

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