// Inclui o cabeçalho da classe Shader.
#include "Shader.h" 
// Inclui bibliotecas para entrada/saída, manipulação de arquivos e streams de string.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// --- CONSTRUTOR ---
// Lê, compila e "linka" (conecta) os shaders para criar um programa de shader.
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. --- LEITURA DOS ARQUIVOS ---
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Garante que os objetos ifstream possam lançar exceções em caso de falha.
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // Abre os arquivos de shader.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        // Lê o conteúdo dos arquivos para os stringstreams.
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        
        // Fecha os manipuladores de arquivo.
        vShaderFile.close();
        fShaderFile.close();
        
        // Converte os stringstreams para strings.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    } catch (std::ifstream::failure e) {
        // Se a leitura falhar, exibe uma mensagem de erro.
        std::cout << "ERRO::SHADER::ARQUIVO_NAO_LIDO\n" 
                  << "Verifique o caminho: " << vertexPath << " ou " << fragmentPath << std::endl;
    }
    
    // Converte o código dos shaders de std::string para um array de caracteres C-style.
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. --- COMPILAÇÃO DOS SHADERS ---
    unsigned int vertex, fragment;

    // --- Vertex Shader ---
    // Cria um objeto de shader do tipo GL_VERTEX_SHADER.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    // Anexa o código-fonte ao objeto de shader.
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    // Compila o shader.
    glCompileShader(vertex);
    // Verifica se houve erros de compilação.
    checkCompileErrors(vertex, "VERTEX"); 

    // --- Fragment Shader ---
    // O processo é o mesmo para o fragment shader.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT"); 

    // 3. --- CRIAÇÃO DO PROGRAMA DE SHADER ---
    // Cria um programa de shader e obtém seu ID.
    ID = glCreateProgram();
    // Anexa os shaders compilados ao programa.
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    // "Linka" (conecta) os shaders para criar o programa executável na GPU.
    glLinkProgram(ID);
    // Verifica se houve erros de linkagem.
    checkCompileErrors(ID, "PROGRAM"); 

    // Após a linkagem, os objetos de shader individuais não são mais necessários.
    // Eles já fazem parte do programa, então podemos excluí-los para liberar recursos.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// --- ATIVAR O SHADER ---
// Define este programa de shader como o ativo para as próximas renderizações.
void Shader::use() { 
    glUseProgram(ID);
}

// --- DEFINIR UNIFORMS ---
// Funções para definir os valores de variáveis 'uniform' no shader.

void Shader::setBool(const std::string &name, bool value) const {
    // Encontra a localização do uniform pelo nome e define seu valor.
    // `glUniform1i` é usado para um único inteiro (e booleanos são convertidos para 0 ou 1).
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    // Define um uniform do tipo inteiro.
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    // Define um uniform do tipo float.
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Para outros tipos, como vetores e matrizes, as funções correspondentes do OpenGL seriam usadas.
// Ex: glUniform3f para um vec3, glUniformMatrix4fv para uma mat4.

// --- VERIFICAR ERROS ---
// Uma função de diagnóstico para verificar erros de compilação e linkagem.
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024]; // Buffer para armazenar a mensagem de erro.

    if (type != "PROGRAM") { // Se estivermos verificando um Vertex ou Fragment Shader
        // Pega o status da compilação.
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            // Se a compilação falhou, obtém a mensagem de log de erro.
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            // Exibe a mensagem de erro.
            std::cout << "ERRO::SHADER::" << type << "::FALHA_COMPILACAO\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else { // Se estivermos verificando o Programa de Shader (linkagem)
        // Pega o status da linkagem.
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            // Se a linkagem falhou, obtém a mensagem de log de erro.
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            // Exibe a mensagem de erro.
            std::cout << "ERRO::SHADER::PROGRAMA::FALHA_LINKAGEM\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}