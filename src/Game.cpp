// Inclui o cabeçalho da classe Game, que contém as declarações.
#include "Game.h"
// Inclui bibliotecas padrão para entrada/saída, números aleatórios e tempo.
#include <iostream>
#include <cstdlib>
#include <ctime>
// Inclui os cabeçalhos da biblioteca GLM para transformações matriciais.
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- FUNÇÃO AUXILIAR ---
// Gera um número inteiro aleatório dentro de um intervalo [min, max].
// É usada para determinar a nova posição da comida.
int randomRange(int min, int max)
{
    // A fórmula `min + rand() % (max - min + 1)` garante uma distribuição uniforme no intervalo.
    return min + (rand() % (max - min + 1));
}

// --- CONSTRUTOR ---
// Inicializa os membros da classe Game.
Game::Game(unsigned int width, unsigned int height)
    // Usa uma lista de inicialização para definir os valores iniciais das variáveis membro.
    : screenWidth(width), screenHeight(height),   // Define as dimensões da tela.
      gridWidth(20), gridHeight(20),             // Define as dimensões do grid do jogo.
      snake(gridWidth / 4, gridHeight / 2),     // Cria a cobra no primeiro quarto do grid.
      food({15, 10}),                           // Posição inicial da comida.
      window(nullptr), shader(nullptr),         // Inicializa ponteiros como nulos.
      VAO(0), VBO(0)                            // Inicializa IDs do OpenGL como 0.
{
    // Chama o método init() para configurar a janela e o OpenGL.
    init();
}

// --- DESTRUTOR ---
// Libera os recursos alocados dinamicamente.
Game::~Game()
{
    // Deleta o objeto shader para evitar vazamento de memória.
    delete shader;
    // Encerra a biblioteca GLFW, liberando todos os recursos que ela alocou.
    glfwTerminate();
}

// --- INICIALIZAÇÃO (INIT) ---
// Configura a janela GLFW, o contexto OpenGL, os shaders e os buffers de vértices.
void Game::init()
{
    // Inicializa a biblioteca GLFW. Se falhar, exibe um erro.
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return;
    }

    // Define as "dicas" (hints) da janela para a versão 3.3 do OpenGL e o perfil Core.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela do jogo. Se falhar, exibe um erro.
    window = glfwCreateWindow(screenWidth, screenHeight, "Snake Game", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return;
    }

    // Define o contexto OpenGL atual para a janela que acabamos de criar.
    glfwMakeContextCurrent(window);
    // Registra a função de callback que será chamada sempre que a janela for redimensionada.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializa a biblioteca GLAD para carregar os ponteiros de função do OpenGL.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return;
    }

    // Inicializa o gerador de números aleatórios com a hora atual para garantir valores diferentes a cada execução.
    srand((unsigned int)time(0));

    // --- CONFIGURAÇÃO DOS VÉRTICES DO QUADRADO (COBRA E COMIDA) ---
    // Um quadrado é formado por dois triângulos. Estes são os vértices de um quadrado centrado em (0,0).
    float vertices[] = {
        // Triângulo 1
        -0.5f, -0.5f, 0.0f,  // Vértice inferior esquerdo
         0.5f, -0.5f, 0.0f,  // Vértice inferior direito
         0.5f,  0.5f, 0.0f,  // Vértice superior direito
        // Triângulo 2
         0.5f,  0.5f, 0.0f,  // Vértice superior direito
        -0.5f,  0.5f, 0.0f,  // Vértice superior esquerdo
        -0.5f, -0.5f, 0.0f   // Vértice inferior esquerdo
    };

    // Gera e configura o Vertex Array Object (VAO) e Vertex Buffer Object (VBO) para o quadrado.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // Ativa o VAO.

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Ativa o VBO.
    // Envia os dados dos vértices para o VBO na GPU.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define como o OpenGL deve interpretar os dados dos vértices no VBO.
    // Atributo 0 (posição), 3 componentes (x,y,z), tipo float, não normalizado, espaçamento entre vértices, offset inicial.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Habilita o atributo de vértice 0.

    // Desvincula os buffers para evitar modificações acidentais.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --- CONFIGURAÇÃO DOS VÉRTICES DO GRID ---
    // Cria os vértices para as linhas verticais e horizontais do grid.
    std::vector<float> gridVertices;
    // Linhas verticais
    for (int i = 0; i <= gridWidth; ++i) {
        gridVertices.push_back((float)i); gridVertices.push_back(0.0f); gridVertices.push_back(0.0f); // Ponto inicial
        gridVertices.push_back((float)i); gridVertices.push_back((float)gridHeight); gridVertices.push_back(0.0f); // Ponto final
    }
    // Linhas horizontais
    for (int i = 0; i <= gridHeight; ++i) {
        gridVertices.push_back(0.0f); gridVertices.push_back((float)i); gridVertices.push_back(0.0f); // Ponto inicial
        gridVertices.push_back((float)gridWidth); gridVertices.push_back((float)i); gridVertices.push_back(0.0f); // Ponto final
    }

    // Gera e configura o VAO e VBO para o grid, de forma similar ao quadrado.
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --- CONFIGURAÇÃO DO SHADER E PROJEÇÃO ---
    // Cria e carrega os shaders (vertex e fragment).
    shader = new Shader("res/shaders/snakeVertex.vert", "res/shaders/snakeColor.frag");
    shader->use(); // Ativa o programa de shader.

    // Obtém a localização das variáveis 'uniform' no shader.
    // 'uniforms' são variáveis globais no shader que podemos definir a partir do nosso código C++.
    projectionLoc = glGetUniformLocation(shader->ID, "projection");
    modelLoc = glGetUniformLocation(shader->ID, "model");
    colorLoc = glGetUniformLocation(shader->ID, "objectColor");

    // Cria uma matriz de projeção ortográfica.
    // Isso mapeia nosso sistema de coordenadas do grid (0 a gridWidth, 0 a gridHeight)
    // para as coordenadas de clipe normalizadas (-1 a 1) que o OpenGL usa.
    glm::mat4 projection = glm::ortho(
        0.0f, (float)gridWidth,    // Esquerda, Direita
        0.0f, (float)gridHeight,   // Baixo, Cima
        -1.0f, 1.0f                // Z-perto, Z-longe
    );

    // Envia a matriz de projeção para o shader. Ela não mudará durante o jogo.
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

// --- CALLBACK DE REDIMENSIONAMENTO ---
// Esta função estática é chamada pelo GLFW sempre que a janela é redimensionada.
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Ajusta o viewport do OpenGL para cobrir a nova dimensão da janela.
    glViewport(0, 0, width, height);
}

// --- LOOP PRINCIPAL DO JOGO (RUN) ---
// Contém o loop que executa até o jogador fechar a janela.
void Game::run()
{
    float lastTime = 0.0f; // Armazena o tempo do último update.
    const float MOVE_INTERVAL = 0.15f; // Intervalo de tempo (em segundos) entre cada movimento da cobra.

    // Loop principal: continua enquanto a janela não deve ser fechada.
    while (!glfwWindowShouldClose(window))
    {
        // 1. Processa entradas do usuário.
        processInput();

        // 2. Atualiza o estado do jogo em intervalos fixos.
        float currentTime = (float)glfwGetTime(); // Obtém o tempo atual.
        if (currentTime - lastTime >= MOVE_INTERVAL)
        {
            update(); // Move a cobra, verifica colisões, etc.
            lastTime = currentTime; // Atualiza o tempo do último movimento.
        }

        // 3. Renderiza o frame atual.
        render();

        // 4. Troca os buffers da janela (double buffering) para exibir o que foi desenhado.
        glfwSwapBuffers(window);
        // 5. Processa eventos do sistema (como fechar a janela, movimento do mouse, etc.).
        glfwPollEvents();
    }
}

// --- PROCESSAMENTO DE ENTRADA ---
// Verifica o estado das teclas e atualiza o estado do jogo.
void Game::processInput()
{
    // Fecha o jogo se a tecla ESC for pressionada.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Muda a direção da cobra com base nas teclas de seta.
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        snake.changeDirection(Direction::UP);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        snake.changeDirection(Direction::DOWN);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        snake.changeDirection(Direction::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        snake.changeDirection(Direction::LEFT);
}

// --- ATUALIZAÇÃO DO JOGO (UPDATE) ---
// Responsável pela lógica do jogo que acontece a cada passo de tempo.
void Game::update()
{
    // Verifica se a cabeça da cobra está na mesma posição da comida.
    bool ateFood = (snake.getHead() == food);

    // Move a cobra. Se 'ateFood' for true, a cobra crescerá.
    snake.move(ateFood);

    // Verifica as condições de fim de jogo.
    if (snake.isOutOfBounds(gridWidth, gridHeight) || snake.isSelfCollision())
    {
        std::cout << "GAME OVER! Pontuação: " << snake.getBody().size() - 1 << std::endl;
        resetGame(); // Reinicia o jogo.
    }

    // Se a cobra comeu, gera uma nova comida.
    if (ateFood)
    {
        food = generateNewFood();
    }
}

// --- RENDERIZAÇÃO ---
// Limpa a tela e desenha todos os elementos do jogo.
void Game::render()
{
    // Limpa o buffer de cor com a cor preta.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Ativa o nosso programa de shader para todos os desenhos deste frame.
    shader->use();

    // Desenha a comida (um quadrado vermelho).
    drawSquare(food, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // Desenha a cobra (uma série de quadrados verdes).
    glm::vec4 snakeColor(0.0f, 1.0f, 0.0f, 1.0f);
    for (const auto &segment : snake.getBody())
    {
        drawSquare(segment, snakeColor);
    }

    // Desenha o grid de fundo.
    // Reseta a matriz 'model' para a matriz identidade.
    // Isso garante que o grid não seja afetado pelas transformações (translação)
    // usadas para desenhar a cobra e a comida.
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // Define a cor do grid (cinza escuro).
    glUniform4f(colorLoc, 0.2f, 0.2f, 0.2f, 1.0f);
    // Ativa o VAO do grid.
    glBindVertexArray(gridVAO);
    // Desenha as linhas do grid.
    glDrawArrays(GL_LINES, 0, (gridWidth + gridHeight + 2) * 2);
    // Desativa o VAO.
    glBindVertexArray(0);
}

// --- REINICIAR O JOGO ---
// Reseta a cobra e a comida para suas posições iniciais.
void Game::resetGame()
{
    snake = Snake(gridWidth / 4, gridHeight / 2);
    food = generateNewFood();
}

// --- GERAR NOVA COMIDA ---
// Encontra uma nova posição aleatória para a comida que não esteja sobre a cobra.
GridPosition Game::generateNewFood()
{
    GridPosition newPosition;
    bool validPosition = false;

    // Continua gerando novas posições até encontrar uma que seja válida.
    while (!validPosition)
    {
        // Gera uma posição aleatória dentro dos limites do grid.
        newPosition.x = randomRange(0, gridWidth - 1);
        newPosition.y = randomRange(0, gridHeight - 1);

        // Assume que a posição é válida até que se prove o contrário.
        validPosition = true;

        // Verifica se a nova posição colide com algum segmento do corpo da cobra.
        for (const auto &segment : snake.getBody())
        {
            if (newPosition == segment)
            {
                validPosition = false; // Posição inválida, tenta novamente.
                break;
            }
        }
    }

    return newPosition;
}

// --- DESENHAR QUADRADO ---
// Desenha um único quadrado na posição e cor especificadas.
void Game::drawSquare(const GridPosition &position, const glm::vec4 &color)
{
    // Envia a cor do quadrado para o shader.
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

    // Cria uma matriz 'model' para posicionar o quadrado no grid.
    // Começa com a matriz identidade.
    glm::mat4 model = glm::mat4(1.0f);
    // Translada o quadrado (que está centrado em 0,0) para a posição correta no grid.
    // Adiciona 0.5f para centralizar o quadrado dentro da célula do grid.
    model = glm::translate(model, glm::vec3((float)position.x + 0.5f,
                                            (float)position.y + 0.5f, 0.0f));

    // Envia a matriz 'model' para o shader.
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Ativa o VAO do quadrado.
    glBindVertexArray(VAO);
    // Desenha o quadrado (que consiste em 6 vértices para 2 triângulos).
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Desativa o VAO.
    glBindVertexArray(0);
}
