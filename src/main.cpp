#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Snake.h"

void processInput(GLFWwindow *window, Snake& snake)
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;

glm::mat4 createOrthographicProjection(float left, float right, float bottom, float top,
                                       float near, float far)
{
    return glm::ortho(left, right, bottom, top, near, far);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Snake Game", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
        // Triângulo 1
        -0.5f, -0.5f, 0.0f, // Inferior Esquerdo
        0.5f, -0.5f, 0.0f,  // Inferior Direito
        0.5f, 0.5f, 0.0f,   // Superior Direito

        // Triângulo 2
        0.5f, 0.5f, 0.0f,  // Superior Direito
        -0.5f, 0.5f, 0.0f, // Superior Esquerdo
        -0.5f, -0.5f, 0.0f // Inferior Esquerdo
    };

    float lastTime = 0.0f;
    const float MOVE_INTERVAL = 0.2f;

    Snake snake(5, 5);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("res/shaders/snakeVertex.vert", "res/shaders/snakeColor.frag");
    shader.use();
    
    unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

    glm::mat4 projection = glm::ortho(
        0.0f, (float)GRID_WIDTH * aspectRatio,
        0.0f, (float)GRID_HEIGHT,
        -1.0f, 1.0f);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, snake);

        float currentTime = (float)glfwGetTime();
        if (currentTime - lastTime >= MOVE_INTERVAL) {
            snake.move(false);

            if (snake.isOutOfBounds(GRID_WIDTH, GRID_HEIGHT) || snake.isSelfCollision()) {
                glfwSetWindowShouldClose(window, true);
            }

            lastTime = currentTime;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        for (const auto& segment : snake.getBody()) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)segment.x + 0.5f, 
                (float)segment.y + 0.5f, 
                0.0f)
            );

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}