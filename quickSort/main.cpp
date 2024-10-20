#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <time.h>
#include "../utils.h"


#define INPUT_FILE "entrada-quicksort.txt"
#define OUTPUT_FILE "resultados.txt"

int vetor[1000000];
int trocas, recursoes;

enum options{
    RANDOM_PIVOT,
    MEDIAN_PIVOT,
    LOMUTO,
    HOARE,
    NUM_OPTIONS 
};

const std::string optionsStrings[NUM_OPTIONS] = {
    "aleatorio",
    "mediana3",
    "lomuto",
    "hoare"
};


void random_pivot(int* array, int inicio, int fim) {
    // Gerar um índice aleatório dentro do intervalo
    srand(time(NULL));
    int randomIndex = rand() % (fim - inicio + 1) + inicio;

    // Trocar o valor aleatório com o primeiro valor do subarray
    std::swap(array[inicio], array[randomIndex]);
}

void median_pivot(int* array, int inicio, int fim) {
    // Encontrar o índice médio
    int indiceMedio = (inicio + fim) / 2;

    // Encontrar o valor mediano
    if (array[inicio] <= array[indiceMedio] && array[inicio] >= array[fim]) {
        
    } else if (array[indiceMedio] <= array[inicio] && array[indiceMedio] >= array[fim]) {
        std::swap(array[inicio], array[indiceMedio]);
    } else {
        std::swap(array[inicio], array[fim]);
    }
}

int partition_lomuto(int C[], int left, int right){
    int chave = C[left];
    int storeindex = left + 1;  // Index of smaller element

    for (int i = left+1; i <= right; i++){
        // If current element is smaller than or
        // equal to pivot
        if (C[i] < chave){
            std::swap(C[i], C[storeindex]);
            trocas++;
            storeindex++;    // increment index of smaller element
        }
    }
    std::swap(C[left], C[storeindex-1]);
    trocas++;

    return (storeindex-1);
}

int partition_hoare(int C[], int left, int right) {
    int chave, i, j;
    chave = C[left]; i = left; j = right;

    while (i<j) {
        //cout << "\nloop = " << loopcount++ << "i - " << i << " j = " << j << endl;
        while(C[j] > chave && i < j) j--;
        C[i] = C[j]; 
        trocas++;
        while(C[i] <= chave && i < j) i++ ;
        C[j] = C[i];
        trocas++;
    }

    C[j] = chave;
    return i;
}

void quicksort(int c[], int i, int f, enum options pivot_method, enum options partition_method){
    int p; //indice do pivo
    if(f > i){
        switch(pivot_method){
            case RANDOM_PIVOT:
            random_pivot(c, i, f);
            break;

            case MEDIAN_PIVOT:
            median_pivot(c, i, f);
            break;

            default:
            std::cerr << "Opção de pivotamento inválida.\n";
            break;
        }

        switch(partition_method){
            case LOMUTO:
            p = partition_lomuto(c, i, f);
            break;

            case HOARE:
            p = partition_hoare(c, i, f);
            break;
            
            default:
            std::cerr << "Opção de particionamento inválida.\n";
            break;
        }

        //p = particiona(c, i, f);
        quicksort(c, i, p-1, pivot_method, partition_method);
        recursoes++;
        quicksort(c, p+1, f, pivot_method, partition_method);
        recursoes++;
    }
}

int main(){
    int i, num_elementos, num_linha;
    
    i = 0;
    trocas = recursoes = i;

    std::string file_dir = OUTPUT_FILE;
    std::ofstream output(file_dir, std::ios::app);
    output << "tamanho,escolha-particionador,particionamento,trocas,recursoes,tempo \n"; 

    for(int num_linha = 1; num_linha < 6; num_linha++){
        lerArquivo(INPUT_FILE, num_linha, vetor, num_elementos);
        
        auto start = std::chrono::high_resolution_clock::now();

        quicksort(vetor, i, num_elementos - 1, MEDIAN_PIVOT, HOARE);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        output << num_elementos << ","; // tamanho
        output << optionsStrings[MEDIAN_PIVOT] << ","; //escolha-particionador(aleatorio, mediana3)
        output << optionsStrings[HOARE] << ",";//particionamento (hoare, lomuto)
        output << trocas << ","; //trocas
        output << recursoes << ","; //recursoes
        output << duration.count() << "\n"; //tempo
    }

    output.close();
}