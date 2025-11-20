// Especifica a versão do GLSL (OpenGL Shading Language) que estamos usando.
#version 330 core

// --- SAÍDA DE FRAGMENTO ---
// Declara a variável de saída do fragment shader. O valor que atribuímos a esta variável
// será a cor final do pixel (ou "fragmento") na tela.
// 'out vec4 FragColor' declara uma variável de saída chamada 'FragColor' do tipo vetor de 4 floats (R, G, B, A).
out vec4 FragColor;

// --- VARIÁVEL GLOBAL (UNIFORM) ---
// Declara um uniform que receberá a cor do objeto do nosso programa C++.
// Como é um uniform, todos os fragmentos de uma mesma chamada de desenho terão a mesma cor.
// Isso é perfeito para o nosso jogo, onde cada parte (segmento da cobra, comida, linha do grid)
// tem uma cor sólida única.
uniform vec4 objectColor;

// A função 'main' é o ponto de entrada do fragment shader.
// Ela é executada para cada fragmento (pixel potencial) gerado pela rasterização
// das primitivas (triângulos, linhas, etc.).
void main()
{
  // A tarefa do fragment shader é determinar a cor de cada fragmento.
  // Neste caso, a lógica é muito simples: apenas atribuímos a cor recebida
  // do uniform 'objectColor' diretamente à variável de saída 'FragColor'.
  FragColor = objectColor;
}