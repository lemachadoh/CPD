#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "../utils.h"

#define INPUT_FILE "entrada-quicksort.txt"
#define OUTPUT_FILE "resultados.txt"

int vetor[1000000]; 
int trocas, recursoes;

enum MetodoParticionamento {
    LOMUTO,
    HOARE,
    NUM_OPTIONS
};

enum MetodoEscolhaPivo {
    RANDOM_PIVOT,
    MEDIAN_PIVOT,
    NUM_SELECTION
};

const std::string particionamentoString[NUM_OPTIONS] = {
    "lomuto",
    "hoare"
};

const std::string escolhaPivoString[NUM_SELECTION] = {
    "aleatorio",
    "mediana3"
};

// Função para selecionar pivô aleatório
void pivoAleatorio(int* arr, int inicio, int fim) {
    int indiceAleatorio = rand() % (fim - inicio + 1) + inicio;
    std::swap(arr[inicio], arr[indiceAleatorio]);
}

// Função de escolha de pivô pela mediana de três
void medianaDeTres(int* arr, int inicio, int fim) {
    int meio = (inicio + fim) / 2;
    // Ordena os três elementos e posiciona a mediana no início
    if (arr[inicio] > arr[meio]) std::swap(arr[inicio], arr[meio]);
    if (arr[inicio] > arr[fim]) std::swap(arr[inicio], arr[fim]);
    if (arr[meio] > arr[fim]) std::swap(arr[meio], arr[fim]);
    std::swap(arr[inicio], arr[meio]); // Coloca a mediana no início
}

// Particionamento de Lomuto
int particionamentoLomuto(int arr[], int esquerda, int direita) {
    int pivo = arr[esquerda];
    int indiceMenor = esquerda + 1;
    for (int i = esquerda + 1; i <= direita; i++) {
        if (arr[i] < pivo) {
            std::swap(arr[i], arr[indiceMenor]);
            trocas++;
            indiceMenor++;
        }
    }
    std::swap(arr[esquerda], arr[indiceMenor - 1]);
    trocas++;
    return indiceMenor - 1;
}

// Particionamento de Hoare corrigido
int particionamentoHoare(int arr[], int esquerda, int direita) {
    int pivo = arr[esquerda];
    int i = esquerda, j = direita;

    while (i < j) {
        while (arr[j] > pivo && i < j) j--;
        arr[i] = arr[j];
        trocas++;
        while (arr[i] <= pivo && i < j) i++;
        arr[j] = arr[i];
        trocas++;
    }
    arr[j] = pivo;
    return j; // Retorna 'j' corretamente, não 'i'
}

// Função de Quicksort corrigida
void quicksort(int arr[], int inicio, int fim, MetodoEscolhaPivo metodoPivo, MetodoParticionamento metodoParticionamento) {
    if (fim > inicio) {
        int indicePivo;
        
        // Escolha do método de pivô
        switch (metodoPivo) {
            case RANDOM_PIVOT:
                pivoAleatorio(arr, inicio, fim);
                break;
            case MEDIAN_PIVOT:
                medianaDeTres(arr, inicio, fim);
                break;
        }

        // Escolha do método de particionamento
        switch (metodoParticionamento) {
            case LOMUTO:
                indicePivo = particionamentoLomuto(arr, inicio, fim);
                break;
            case HOARE:
                indicePivo = particionamentoHoare(arr, inicio, fim);
                break;
        }

        // Recursão para ordenar as duas sublistas
        quicksort(arr, inicio, indicePivo - 1, metodoPivo, metodoParticionamento);
        recursoes++;
        quicksort(arr, indicePivo + 1, fim, metodoPivo, metodoParticionamento);
        recursoes++;
    }
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios uma única vez

    int elementos, linha;
    trocas = recursoes = 0;

    std::ofstream output(OUTPUT_FILE, std::ios::app);
    output << "tamanho,escolhaPivo,particionamento,trocas,recursoes,tempo \n"; 

    for (int linha = 1; linha < 6; linha++) {
        for (int metodoPivo = 0; metodoPivo < NUM_SELECTION; metodoPivo++) {
            for (int metodoParticionamento = 0; metodoParticionamento < NUM_OPTIONS; metodoParticionamento++) {
                
                trocas = recursoes = 0;
                lerArquivo(INPUT_FILE, linha, vetor, elementos);

                auto inicio = std::chrono::high_resolution_clock::now();

                quicksort(vetor, 0, elementos - 1, (MetodoEscolhaPivo)metodoPivo, (MetodoParticionamento)metodoParticionamento);

                auto fim = std::chrono::high_resolution_clock::now();
                auto duracao = std::chrono::duration<double, std::milli>(fim - inicio);

                output << elementos << ",";
                output << escolhaPivoString[metodoPivo] << ",";
                output << particionamentoString[metodoParticionamento] << ",";
                output << trocas << ",";
                output << recursoes << ",";
                output << duracao.count() << "\n";
            }
        }
    }

    output.close();
}
