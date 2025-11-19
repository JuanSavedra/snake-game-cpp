#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Helper function, we can move this later if needed
int randomRange(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

Game::Game(unsigned int width, unsigned int height)
    : screenWidth(width), screenHeight(height),
      gridWidth(20), gridHeight(20),
      snake(gridWidth / 4, gridHeight / 2),
      food({15, 10}),
      window(nullptr), shader(nullptr),
      VAO(0), VBO(0)
{
    init();
}

Game::~Game()
{
    delete shader;
    glfwTerminate();
}

void Game::init()
{
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Snake Game", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return;
    }

    srand((unsigned int)time(0));

    float vertices[] = {
        //Triângulo 1
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        //Triângulo 2
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::vector<float> gridVertices;
    for (int i = 0; i <= gridWidth; ++i) {
        gridVertices.push_back((float)i); gridVertices.push_back(0.0f); gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i); gridVertices.push_back((float)gridHeight); gridVertices.push_back(0.0f);
    }
    for (int i = 0; i <= gridHeight; ++i) {
        gridVertices.push_back(0.0f); gridVertices.push_back((float)i); gridVertices.push_back(0.0f);
        gridVertices.push_back((float)gridWidth); gridVertices.push_back((float)i); gridVertices.push_back(0.0f);
    }

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = new Shader("res/shaders/snakeVertex.vert", "res/shaders/snakeColor.frag");
    shader->use();

    projectionLoc = glGetUniformLocation(shader->ID, "projection");
    modelLoc = glGetUniformLocation(shader->ID, "model");
    colorLoc = glGetUniformLocation(shader->ID, "objectColor");

    glm::mat4 projection = glm::ortho(
        0.0f, (float)gridWidth,
        0.0f, (float)gridHeight,
        -1.0f, 1.0f);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::run()
{
    float lastTime = 0.0f;
    const float MOVE_INTERVAL = 0.15f;

    while (!glfwWindowShouldClose(window))
    {
        processInput();

        float currentTime = (float)glfwGetTime();
        if (currentTime - lastTime >= MOVE_INTERVAL)
        {
            update();
            lastTime = currentTime;
        }

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        snake.changeDirection(Direction::UP);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        snake.changeDirection(Direction::DOWN);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        snake.changeDirection(Direction::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        snake.changeDirection(Direction::LEFT);
}

void Game::update()
{
    bool ateFood = (snake.getHead() == food);

    snake.move(ateFood);

    if (snake.isOutOfBounds(gridWidth, gridHeight) || snake.isSelfCollision())
    {
        std::cout << "GAME OVER! Pontuação: " << snake.getBody().size() - 1 << std::endl;
        resetGame();
    }

    if (ateFood)
    {
        food = generateNewFood();
    }
}

void Game::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader->use();

    // Food render
    drawSquare(food, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // Snake render
    glm::vec4 snakeColor(0.0f, 1.0f, 0.0f, 1.0f);
    for (const auto &segment : snake.getBody())
    {
        drawSquare(segment, snakeColor);
    }

    // Grid render
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorLoc, 0.2f, 0.2f, 0.2f, 1.0f); // Dark gray for grid lines
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, (gridWidth + gridHeight + 2) * 2);
    glBindVertexArray(0);
}

void Game::resetGame()
{
    snake = Snake(gridWidth / 4, gridHeight / 2);
    food = generateNewFood();
}

GridPosition Game::generateNewFood()
{
    GridPosition newPosition;
    bool validPosition = false;

    while (!validPosition)
    {
        newPosition.x = randomRange(0, gridWidth - 1);
        newPosition.y = randomRange(0, gridHeight - 1);

        validPosition = true;

        for (const auto &segment : snake.getBody())
        {
            if (newPosition == segment)
            {
                validPosition = false;
                break;
            }
        }
    }

    return newPosition;
}

void Game::drawSquare(const GridPosition &position, const glm::vec4 &color)
{
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3((float)position.x + 0.5f,
                                            (float)position.y + 0.5f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
