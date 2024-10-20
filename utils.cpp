#include <fstream>
#include <sstream>
#include "utils.h"
#include <iostream>  


void lerArquivo(const std::string& nomeArquivo, int linha, int* numeros, int& numElementos) {
    std::ifstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        std::string linhaAtual;
        for (int i = 1; i <= linha; ++i) {
            std::getline(arquivo, linhaAtual);
            if (i == linha) {
                std::istringstream iss(linhaAtual);
                iss >> numElementos; // Lê o primeiro número da linha
                for (int j = 0; j < numElementos; ++j) {
                    iss >> numeros[j]; // Lê os números subsequentes
                }
            }
        }
        //std::cout << "Sucesso na leitura do arquivo.\n";
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
    }
}