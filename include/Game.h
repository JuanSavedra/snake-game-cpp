#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Snake.h"
#include "Shader.h"

class Game {
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    void run();

private:
    // Game state
    Snake snake;
    GridPosition food;
    const unsigned int screenWidth;
    const unsigned int screenHeight;
    const int gridWidth;
    const int gridHeight;

    // GLFW window
    GLFWwindow* window;

    // Shader
    Shader* shader;

    // OpenGL objects
    unsigned int VAO, VBO;
    unsigned int gridVAO, gridVBO;
    unsigned int projectionLoc;
    unsigned int modelLoc;
    unsigned int colorLoc;

    // Private methods
    void init();
    void processInput();
    void update();
    void render();

    void resetGame();
    GridPosition generateNewFood();
    void drawSquare(const GridPosition& position, const glm::vec4& color);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif
