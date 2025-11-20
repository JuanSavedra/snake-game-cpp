// Inclui o cabeçalho da classe Game, que contém toda a lógica principal do jogo.
#include "Game.h"

// A função 'main' é o ponto de entrada de qualquer programa C++.
// A execução do programa começa aqui.
int main()
{
    // Cria uma instância (um objeto) da classe Game.
    // O construtor de Game é chamado com as dimensões da janela (800 pixels de largura, 600 de altura).
    // Neste ponto, o método Game::init() é chamado de dentro do construtor,
    // configurando a janela, OpenGL, shaders e tudo o que é necessário para o jogo começar.
    Game game(800, 600);

    // Chama o método 'run' do objeto 'game'.
    // Este método contém o loop principal do jogo (game loop), que continuará
    // executando até que o jogador decida fechar a janela.
    // O loop principal é responsável por processar entradas, atualizar o estado do jogo e renderizar os gráficos.
    game.run();

    // Quando o loop principal em 'game.run()' termina (ou seja, a janela é fechada),
    // o programa continua a partir daqui.
    // O 'return 0' indica ao sistema operacional que o programa foi encerrado com sucesso.
    return 0;
}
