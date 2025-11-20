// Inclui o cabeçalho da classe Snake.
#include "Snake.h"
// Incluído para depuração, pode ser removido em uma versão final.
#include <iostream>

// --- CONSTRUTOR ---
// Cria a cobra, definindo sua posição e direção iniciais.
Snake::Snake(int startX, int startY)
{
  // Adiciona a cabeça como o primeiro segmento do corpo.
  body.push_back({startX, startY});
  // Define a direção inicial e a próxima direção como 'DIREITA'.
  currentDirection = Direction::RIGHT;
  nextDirection = Direction::RIGHT;
}

// --- MOVIMENTO DA COBRA ---
// Atualiza a posição da cobra a cada passo do jogo.
void Snake::move(bool grow)
{
  // Antes de mover, atualiza a direção atual com a próxima direção solicitada pelo jogador.
  // Isso torna o controle mais responsivo, pois a mudança de direção é registrada
  // e depois aplicada no momento certo.
  currentDirection = nextDirection;
  
  // Calcula a posição da nova cabeça com base na posição da cabeça atual.
  GridPosition newHead = getHead();

  // Usa um switch para determinar a nova posição da cabeça com base na direção.
  switch (currentDirection)
  {
  case Direction::UP:
    newHead.y += 1; // No nosso sistema de coordenadas, Y aumenta para cima.
    break;
  case Direction::DOWN:
    newHead.y -= 1; // Y diminui para baixo.
    break;
  case Direction::RIGHT:
    newHead.x += 1; // X aumenta para a direita.
    break;
  case Direction::LEFT:
    newHead.x -= 1; // X diminui para a esquerda.
    break;
  }

  // Adiciona a nova cabeça no início do vetor 'body'.
  // O corpo da cobra agora "se moveu" para frente.
  body.insert(body.begin(), newHead);

  // Se a cobra não comeu uma fruta ('grow' é falso), remove o último segmento.
  // Isso faz com que a cobra mantenha seu tamanho e dê a ilusão de movimento.
  // Se 'grow' for verdadeiro, o último segmento não é removido, e a cobra cresce.
  if (!grow)
  {
    body.pop_back();
  }
}

// --- MUDANÇA DE DIREÇÃO ---
// Registra a nova direção solicitada pelo jogador.
void Snake::changeDirection(Direction newDirection)
{
  // Adiciona uma lógica para impedir que a cobra se inverta sobre si mesma.
  // Por exemplo, se está indo para cima, não pode ir para baixo imediatamente.
  if (currentDirection == Direction::UP && newDirection == Direction::DOWN)
    return; // Ignora a nova direção
  if (currentDirection == Direction::DOWN && newDirection == Direction::UP)
    return;
  if (currentDirection == Direction::LEFT && newDirection == Direction::RIGHT)
    return;
  if (currentDirection == Direction::RIGHT && newDirection == Direction::LEFT)
    return;

  // Se a nova direção for válida, armazena em 'nextDirection'.
  // Ela será aplicada no próximo chamado do método 'move'.
  nextDirection = newDirection;
}

// --- VERIFICAÇÃO DE LIMITES ---
// Checa se a cabeça da cobra saiu da área do grid.
bool Snake::isOutOfBounds(int gridWidth, int gridHeight) const
{
  // Obtém uma referência constante à cabeça para evitar cópias.
  const GridPosition& head = getHead();
  // Retorna verdadeiro se a coordenada X ou Y da cabeça estiver fora do intervalo [0, gridWidth-1] ou [0, gridHeight-1].
  return head.x < 0 || head.x >= gridWidth ||
         head.y < 0 || head.y >= gridHeight;
}

// --- VERIFICAÇÃO DE AUTO-COLISÃO ---
// Checa se a cabeça da cobra colidiu com qualquer outra parte do seu corpo.
bool Snake::isSelfCollision() const
{
  // A colisão só é possível se a cobra tiver 4 ou mais segmentos.
  // Com 3 ou menos, a cabeça não pode alcançar o rabo em um movimento.
  if (body.size() < 4)
  {
    return false;
  }

  // Obtém a posição da cabeça.
  const GridPosition &head = getHead();
  // Itera por todos os segmentos do corpo, *começando do segundo* (índice 1).
  for (size_t i = 1; i < body.size(); i++)
  {
    // Se a posição da cabeça for igual à de qualquer outro segmento, houve colisão.
    if (head == body[i])
    {
      return true;
    }
  }

  // Se o loop terminar sem encontrar colisões, retorna falso.
  return false;
}

// --- OBTER CABEÇA ---
// Retorna a posição do primeiro elemento do corpo, que é a cabeça.
GridPosition Snake::getHead() const {
  // body[0] é sempre a cabeça da cobra.
  return body[0];
}