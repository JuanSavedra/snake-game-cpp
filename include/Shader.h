// Impede que o cabeçalho seja incluído várias vezes em uma mesma compilação.
#ifndef SHADER_H
#define SHADER_H

// Inclui a biblioteca de strings da STL e o cabeçalho do GLAD para as funções do OpenGL.
#include <string>
#include <glad/glad.h>

// A classe Shader encapsula a lógica para carregar, compilar e gerenciar
// um programa de shader do OpenGL (que consiste em um vertex e um fragment shader).
class Shader {
  public:
    // O ID do programa de shader, que é um número de referência usado pelo OpenGL.
    unsigned int ID;

    // Construtor: lê e compila os shaders a partir dos caminhos dos arquivos fornecidos.
    // vertexPath: caminho para o arquivo do vertex shader (código .vert).
    // fragmentPath: caminho para o arquivo do fragment shader (código .frag).
    Shader(const char* vertexPath, const char* fragmentPath);

    // Ativa este programa de shader para ser usado nas próximas operações de renderização.
    void use();

    // --- FUNÇÕES 'SETTER' PARA UNIFORMS ---
    // Métodos convenientes para definir os valores de variáveis 'uniform' no shader.
    // Uniforms são variáveis globais no shader que podem ser alteradas pelo código C++.
    // O 'const' no final indica que esses métodos não alteram o estado do objeto Shader (além do estado do OpenGL).

    // Define um uniform booleano.
    void setBool(const std::string &name, bool value) const;
    // Define um uniform inteiro.
    void setInt(const std::string &name, int value) const;
    // Define um uniform de ponto flutuante.
    void setFloat(const std::string &name, float value) const;
    
  private:
    // Método auxiliar privado para verificar erros de compilação ou de "linkagem" (linking) dos shaders.
    // shader: o ID do shader ou do programa.
    // type: uma string que descreve o que está sendo verificado ("VERTEX", "FRAGMENT", ou "PROGRAM").
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif