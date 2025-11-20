// Especifica a versão do GLSL (OpenGL Shading Language) que estamos usando.
// '330' corresponde ao OpenGL 3.3, e 'core' significa que estamos usando o perfil Core.
#version 330 core

// --- ENTRADA DE VÉRTICE (ATRIBUTO) ---
// Declara um atributo de vértice de entrada. Atributos são dados que são fornecidos
// para cada vértice individualmente (como posição, cor, coordenada de textura, etc.).
// 'layout (location = 0)' especifica o "endereço" deste atributo. Isso corresponde
// ao 'glVertexAttribPointer(0, ...)' e 'glEnableVertexAttribArray(0)' em nosso código C++.
// 'in vec3 aPos' declara uma variável de entrada chamada 'aPos' do tipo vetor de 3 floats (x, y, z).
// Ela receberá a posição de cada vértice do nosso VBO (Vertex Buffer Object).
layout (location = 0) in vec3 aPos;

// --- VARIÁVEIS GLOBAIS (UNIFORMS) ---
// Uniforms são variáveis globais que têm o mesmo valor para todos os vértices
// processados por uma única chamada de desenho. Elas são definidas pelo nosso programa C++.
uniform mat4 projection; // A matriz de projeção (ortográfica, no nosso caso).
uniform mat4 model;      // A matriz de modelo (que posiciona, rotaciona e escala nosso objeto).

// A função 'main' é o ponto de entrada do shader.
// Ela é executada para cada vértice que enviamos para a GPU.
void main() 
{
  // --- TRANSFORMAÇÃO DE COORDENADAS ---
  // A principal tarefa do vertex shader é calcular a posição final de cada vértice
  // no espaço de clipe (um cubo normalizado de -1 a 1 em cada eixo).

  // 'gl_Position' é uma variável de saída especial do vertex shader.
  // O valor que atribuímos a ela será a posição final do vértice na tela.
  // Ela é um 'vec4' porque as transformações 3D são feitas com coordenadas homogêneas (x, y, z, w).

  // A transformação é feita multiplicando as matrizes na ordem inversa:
  // 1. 'vec4(aPos, 1.0)': Converte a posição do vértice (vec3) para uma coordenada homogênea (vec4).
  //    O 'w' é definido como 1.0 para que as translações funcionem corretamente.
  // 2. 'model * ...': Aplica a matriz de modelo. Isso transforma o vértice de seu espaço local
  //    (por exemplo, o quadrado centrado em 0,0) para o espaço do mundo (a posição no grid).
  // 3. 'projection * ...': Aplica a matriz de projeção. Isso transforma o vértice do espaço do mundo
  //    para o espaço de clipe, que é o que o OpenGL usa para mapear tudo para a tela.
  gl_Position = projection * model * vec4(aPos, 1.0);
}