// Impede que o cabeçalho seja incluído várias vezes em uma mesma compilação.
#ifndef SNAKE_H
#define SNAKE_H

// Inclui a biblioteca de vetores da STL para armazenar o corpo da cobra.
#include <vector>

// Estrutura para representar uma posição (x, y) no grid.
// É mais simples que uma classe e serve bem para agrupar dados.
struct GridPosition {
    int x; // Coordenada X
    int y; // Coordenada Y

    // Sobrecarga do operador de igualdade (==).
    // Permite comparar duas instâncias de GridPosition diretamente (ex: pos1 == pos2).
    // É 'const' porque não modifica nenhum dos objetos que está comparando.
    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y;
    }
};

// Enumeração de classe para representar as possíveis direções da cobra.
// Usar 'enum class' é mais seguro em C++ moderno do que 'enum' tradicional,
// pois evita conflitos de nomes e conversões implícitas para inteiros.
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// A classe Snake gerencia a lógica, o estado e o comportamento da cobra.
class Snake {
public:
    // Construtor: cria uma cobra com uma posição inicial (cabeça).
    Snake(int startX, int startY);

    // Move a cobra na direção atual.
    // O parâmetro 'grow' indica se a cobra deve crescer (ou seja, se comeu uma fruta).
    void move(bool grow);

    // Altera a próxima direção da cobra, com lógica para evitar que ela se inverta.
    void changeDirection(Direction newDirection);
    
    // Verifica se a cabeça da cobra está fora dos limites do grid.
    bool isOutOfBounds(int gridWidth, int gridHeight) const;
    // Verifica se a cabeça da cobra colidiu com qualquer outra parte de seu corpo.
    bool isSelfCollision() const;

    // --- MÉTODOS DE ACESSO (GETTERS) ---
    // São 'const' porque apenas retornam dados e não modificam o estado do objeto Snake.

    // Retorna a posição da cabeça da cobra.
    GridPosition getHead() const;
    // Retorna uma referência constante ao vetor que representa o corpo da cobra.
    // A referência é mais eficiente que uma cópia, e 'const' garante que o corpo não seja modificado externamente.
    const std::vector<GridPosition>& getBody() const { return body; }
    // Retorna a direção atual da cobra.
    Direction getCurrentDirection() const { return currentDirection; }

private:
    // Vetor de posições que armazena os segmentos do corpo da cobra.
    // O primeiro elemento (body[0]) é a cabeça.
    std::vector<GridPosition> body;
    // A direção em que a cobra está se movendo atualmente.
    Direction currentDirection;
    // A próxima direção que a cobra tomará. Usado para registrar a entrada do jogador
    // antes que o próximo movimento aconteça, tornando os controles mais responsivos.
    Direction nextDirection; 
};

#endif