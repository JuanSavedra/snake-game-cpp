// Impede que o cabeçalho seja incluído várias vezes em uma mesma compilação.
#ifndef GAME_H
#define GAME_H

// Inclui as bibliotecas necessárias para o jogo.
#include <glad/glad.h>   // Para funções do OpenGL.
#include <GLFW/glfw3.h>  // Para criar janelas e gerenciar entradas.
#include <glm/glm.hpp>   // Para operações matemáticas com vetores e matrizes.
#include "Snake.h"       // Inclui a definição da classe Snake.
#include "Shader.h"      // Inclui a definição da classe Shader.

// A classe Game é responsável por gerenciar todo o ciclo de vida e estado do jogo.
class Game {
public:
    // Construtor: inicializa o jogo com uma largura e altura de tela específicas.
    Game(unsigned int width, unsigned int height);
    // Destrutor: libera os recursos alocados pelo jogo.
    ~Game();

    // Inicia e executa o loop principal do jogo.
    void run();

private:
    // --- ESTADO DO JOGO ---
    Snake snake;                          // Objeto que representa a cobra.
    GridPosition food;                    // Posição da comida no grid.
    const unsigned int screenWidth;       // Largura da janela em pixels.
    const unsigned int screenHeight;      // Altura da janela em pixels.
    const int gridWidth;                  // Largura do grid do jogo (em unidades).
    const int gridHeight;                 // Altura do grid do jogo (em unidades).

    // --- JANELA (GLFW) ---
    GLFWwindow* window;                   // Ponteiro para a janela criada pelo GLFW.

    // --- SHADER ---
    Shader* shader;                       // Ponteiro para o objeto de shader que desenha tudo na tela.

    // --- OBJETOS OPENGL ---
    unsigned int VAO, VBO;                // VAO/VBO para os quadrados (corpo da cobra e comida).
    unsigned int gridVAO, gridVBO;        // VAO/VBO para as linhas do grid.
    unsigned int projectionLoc;           // Localização do uniform 'projection' no shader.
    unsigned int modelLoc;                // Localização do uniform 'model' no shader.
    unsigned int colorLoc;                // Localização do uniform 'objectColor' no shader.

    // --- MÉTODOS PRIVADOS ---
    // Inicializa a janela, OpenGL, shaders e os objetos do jogo.
    void init();
    // Processa as entradas do usuário (teclado).
    void processInput();
    // Atualiza o estado do jogo (movimento da cobra, colisões, etc.).
    void update();
    // Desenha todos os elementos do jogo na tela.
    void render();

    // Reinicia o jogo para o estado inicial.
    void resetGame();
    // Gera uma nova posição para a comida, garantindo que não apareça sobre a cobra.
    GridPosition generateNewFood();
    // Desenha um único quadrado no grid (usado para a cobra e a comida).
    void drawSquare(const GridPosition& position, const glm::vec4& color);

    // Função de callback estática para redimensionar a janela.
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif
